#include "wifi_provisioning.h"
#include "portal_material.h"

#define WIFI_TIMEOUT_MS 20000
#define AP_TIMEOUT_MS 300000  // 5 minutes

WiFiProvisioning::WiFiProvisioning() : server(nullptr), apMode(false) {}

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
    // Reserved for future use (e.g., AP timeout handling)
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

    // Setup web server
    setupWebServer();
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
    Serial.println("Web server started");
}
