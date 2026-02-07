#include "wifi_provisioning.h"

#define WIFI_TIMEOUT_MS 20000
#define AP_TIMEOUT_MS 300000  // 5 minutes
#define DNS_PORT 53

WiFiProvisioning::WiFiProvisioning() : server(nullptr), dnsServer(nullptr), apMode(false) {}

bool WiFiProvisioning::begin() {
    String ssid, password;

    // Try to load saved credentials
    if (loadCredentials(ssid, password)) {
        Serial.println("Found saved WiFi credentials");
        Serial.printf("SSID: %s\n", ssid.c_str());

        if (connectToWiFi(ssid, password)) {
            Serial.println("Connected to saved WiFi network");
            apMode = false;
            return true;
        }

        Serial.println("Failed to connect to saved network");
    }

    // No credentials or connection failed - start config portal
    Serial.println("Starting WiFi configuration portal");
    startConfigPortal();
    return false;
}

void WiFiProvisioning::loop() {
    // Process DNS requests in AP mode
    if (apMode && dnsServer) {
        dnsServer->processNextRequest();
    }
}

bool WiFiProvisioning::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiProvisioning::getSSID() {
    return WiFi.SSID();
}

String WiFiProvisioning::getIP() {
    return WiFi.localIP().toString();
}

int WiFiProvisioning::getRSSI() {
    return WiFi.RSSI();
}

void WiFiProvisioning::reset() {
    Serial.println("Resetting WiFi credentials");
    prefs.begin("wifi", false);
    prefs.clear();
    prefs.end();
    WiFi.disconnect(true);
    ESP.restart();
}

bool WiFiProvisioning::loadCredentials(String& ssid, String& password) {
    prefs.begin("wifi", true);  // Read-only
    ssid = prefs.getString("ssid", "");
    password = prefs.getString("password", "");
    prefs.end();

    return (ssid.length() > 0);
}

void WiFiProvisioning::saveCredentials(const String& ssid, const String& password) {
    prefs.begin("wifi", false);  // Read-write
    prefs.putString("ssid", ssid);
    prefs.putString("password", password);
    prefs.end();
    Serial.println("WiFi credentials saved");
}

bool WiFiProvisioning::connectToWiFi(const String& ssid, const String& password) {
    Serial.printf("Connecting to WiFi: %s\n", ssid.c_str());

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected!");
        Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("Signal Strength: %d dBm\n", WiFi.RSSI());
        return true;
    }

    return false;
}

void WiFiProvisioning::startConfigPortal() {
    apMode = true;

    // Create unique AP name
    String apName = "SmartLight-" + String((uint32_t)ESP.getEfuseMac(), HEX);
    apName.toUpperCase();

    Serial.println("\n========================================");
    Serial.println("WiFi Configuration Portal");
    Serial.println("========================================");
    Serial.printf("AP Name: %s\n", apName.c_str());
    Serial.println("AP Password: (none)");
    Serial.println("IP Address: 192.168.4.1");
    Serial.println("\nConnect to this network and open:");
    Serial.println("http://192.168.4.1");
    Serial.println("========================================\n");

    // Start Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apName.c_str());

    // Small delay to let AP start
    delay(100);

    // Setup DNS server (captive portal)
    setupDNS();

    // Setup web server
    setupWebServer();
}

void WiFiProvisioning::setupDNS() {
    if (dnsServer) {
        delete dnsServer;
    }

    dnsServer = new DNSServer();

    // Redirect all DNS queries to this device's IP (192.168.4.1)
    dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

    Serial.println("DNS Server started for captive portal");
}

String getPortalHTML() {
    return R"html(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>WiFi Setup</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: #f0f0f0; }
        .container { max-width: 400px; margin: 0 auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        h1 { color: #333; text-align: center; margin-bottom: 30px; }
        .form-group { margin-bottom: 20px; }
        label { display: block; margin-bottom: 5px; color: #666; font-weight: bold; }
        input[type="text"], input[type="password"], select { width: 100%; padding: 10px; border: 2px solid #ddd; border-radius: 5px; font-size: 16px; box-sizing: border-box; }
        input[type="text"]:focus, input[type="password"]:focus, select:focus { outline: none; border-color: #667eea; }
        button { width: 100%; padding: 12px; background: #667eea; color: white; border: none; border-radius: 5px; font-size: 16px; font-weight: bold; cursor: pointer; }
        button:hover { background: #5568d3; }
        button:disabled { background: #ccc; cursor: not-allowed; }
        .status { margin-top: 15px; padding: 10px; border-radius: 5px; display: none; }
        .status.success { background: #d4edda; color: #155724; display: block; }
        .status.error { background: #f8d7da; color: #721c24; display: block; }
        .scanning { text-align: center; color: #666; padding: 10px; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Smart Home Light<br>WiFi Setup</h1>
        <form id="wifiForm">
            <div class="form-group">
                <label for="ssid">WiFi Network</label>
                <select id="ssid" name="ssid" required>
                    <option value="">Scanning...</option>
                </select>
            </div>
            <div class="form-group">
                <label for="password">Password</label>
                <input type="password" id="password" name="password" required>
            </div>
            <button type="submit" id="submitBtn">Connect</button>
            <div id="status" class="status"></div>
        </form>
    </div>
    <script>
        fetch('/scan').then(r => r.json()).then(networks => {
            const select = document.getElementById('ssid');
            select.innerHTML = '<option value="">Select a network...</option>';
            networks.forEach(n => {
                const option = document.createElement('option');
                option.value = n.ssid;
                option.textContent = n.ssid + ' (' + n.rssi + ' dBm)';
                select.appendChild(option);
            });
        }).catch(e => {
            document.getElementById('ssid').innerHTML = '<option value="">Scan failed</option>';
        });

        document.getElementById('wifiForm').addEventListener('submit', async (e) => {
            e.preventDefault();
            const ssid = document.getElementById('ssid').value;
            const password = document.getElementById('password').value;
            const submitBtn = document.getElementById('submitBtn');
            const status = document.getElementById('status');

            if (!ssid) {
                status.className = 'status error';
                status.textContent = 'Please select a network';
                return;
            }

            submitBtn.disabled = true;
            submitBtn.textContent = 'Connecting...';
            status.className = 'status';
            status.style.display = 'none';

            try {
                const response = await fetch('/connect', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                    body: 'ssid=' + encodeURIComponent(ssid) + '&password=' + encodeURIComponent(password)
                });

                const result = await response.json();

                if (result.success) {
                    status.className = 'status success';
                    status.textContent = 'Connected! Device will restart...';
                    setTimeout(() => { window.location.reload(); }, 3000);
                } else {
                    status.className = 'status error';
                    status.textContent = 'Connection failed: ' + (result.message || 'Unknown error');
                    submitBtn.disabled = false;
                    submitBtn.textContent = 'Connect';
                }
            } catch (error) {
                status.className = 'status error';
                status.textContent = 'Request failed: ' + error.message;
                submitBtn.disabled = false;
                submitBtn.textContent = 'Connect';
            }
        });
    </script>
</body>
</html>
    )html";
}

void WiFiProvisioning::setupWebServer() {
    if (server) {
        delete server;
    }

    server = new AsyncWebServer(80);

    // Get portal HTML once
    String portalHTML = getPortalHTML();

    // Captive Portal Detection Endpoints - Serve portal page directly
    // Android
    server->on("/generate_204", HTTP_GET, [portalHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", portalHTML);
    });
    server->on("/gen_204", HTTP_GET, [portalHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", portalHTML);
    });

    // iOS/macOS - Must return specific content
    server->on("/hotspot-detect.html", HTTP_GET, [portalHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", portalHTML);
    });
    server->on("/library/test/success.html", HTTP_GET, [portalHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", portalHTML);
    });

    // Windows
    server->on("/connecttest.txt", HTTP_GET, [portalHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", portalHTML);
    });
    server->on("/ncsi.txt", HTTP_GET, [portalHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", portalHTML);
    });

    // Serve configuration page at root
    server->on("/", HTTP_GET, [portalHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", portalHTML);
    });

    // Scan for WiFi networks
    server->on("/scan", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("Scanning for WiFi networks...");
        int n = WiFi.scanNetworks();
        String json = "[";

        for (int i = 0; i < n; i++) {
            if (i > 0) json += ",";
            json += "{\"ssid\":\"" + WiFi.SSID(i) + "\",\"rssi\":" + String(WiFi.RSSI(i)) + "}";
        }
        json += "]";

        request->send(200, "application/json", json);
    });

    // Handle WiFi connection request
    server->on("/connect", HTTP_POST, [this](AsyncWebServerRequest *request) {
        String ssid = "";
        String password = "";

        if (request->hasParam("ssid", true)) {
            ssid = request->getParam("ssid", true)->value();
        }
        if (request->hasParam("password", true)) {
            password = request->getParam("password", true)->value();
        }

        if (ssid.length() == 0) {
            request->send(400, "application/json", "{\"success\":false,\"message\":\"SSID required\"}");
            return;
        }

        Serial.printf("Attempting to connect to: %s\n", ssid.c_str());

        // Save credentials
        saveCredentials(ssid, password);

        // Try to connect
        bool connected = connectToWiFi(ssid, password);

        if (connected) {
            request->send(200, "application/json", "{\"success\":true}");
            delay(1000);
            ESP.restart();
        } else {
            request->send(200, "application/json", "{\"success\":false,\"message\":\"Failed to connect\"}");
        }
    });

    // Catch-all handler - serve portal page for all unmatched requests
    server->onNotFound([portalHTML](AsyncWebServerRequest *request) {
        // Log request for debugging
        Serial.printf("Captive portal request: %s %s\n",
                     request->methodToString(),
                     request->url().c_str());

        // Serve portal page for any unmatched request
        request->send(200, "text/html", portalHTML);
    });

    server->begin();
    Serial.println("Web server started with captive portal support");
}
