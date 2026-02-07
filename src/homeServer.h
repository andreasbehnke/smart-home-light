#ifndef HOME_SERVER_H
#define HOME_SERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

// Setup the home web server when connected to WiFi
void setupHomeServer(AsyncWebServer*& server);

// Generate the home page HTML
String getHomeHTML();

#endif
