#include "homeServer.h"
#include "web_material.h"
#include <WiFi.h>

String getHomeHTML() {
    String html = MaterialPage::getHeader("Smart Home Light");
    html += MaterialPage::getAppBar("Smart Home Light", "WiFi Connected");
    html += MaterialPage::getFooter();
    return html;
}

void setupHomeServer(AsyncWebServer*& server) {
    if (server) {
        delete server;
    }

    server = new AsyncWebServer(80);

    // Get home HTML once
    String homeHTML = getHomeHTML();

    // Serve homepage at root
    server->on("/", HTTP_GET, [homeHTML](AsyncWebServerRequest *request) {
        request->send(200, "text/html", homeHTML);
    });

    server->begin();
    Serial.println("Home web server started");
    Serial.printf("Access at: http://%s\n", WiFi.localIP().toString().c_str());
}
