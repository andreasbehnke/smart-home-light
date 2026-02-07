#ifndef WIFI_PROVISIONING_H
#define WIFI_PROVISIONING_H

#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

class WiFiProvisioning {
public:
    WiFiProvisioning();
    bool begin();
    void loop();  // Must be called in main loop for DNS server
    bool isConnected();
    String getSSID();
    String getIP();
    int getRSSI();
    void reset();

private:
    Preferences prefs;
    AsyncWebServer* server;
    DNSServer* dnsServer;
    bool apMode;

    bool loadCredentials(String& ssid, String& password);
    void saveCredentials(const String& ssid, const String& password);
    bool connectToWiFi(const String& ssid, const String& password);
    void startConfigPortal();
    void setupWebServer();
    void setupDNS();
};

#endif
