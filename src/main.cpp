#include <Arduino.h>
#include "wifi_provisioning.h"

WiFiProvisioning wifiProv;

void printSystemInfo();
void handleSerialCommands();

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n\n========================================");
    Serial.println("Smart Home Light Controller");
    Serial.println("========================================");

    printSystemInfo();

    // Setup WiFi with provisioning
    Serial.println("Initializing WiFi...");
    wifiProv.begin();

    Serial.println("========================================\n");
}

void loop() {
    // Handle WiFi provisioning
    wifiProv.loop();

    // Handle serial commands
    handleSerialCommands();

    static unsigned long lastPrint = 0;
    unsigned long currentMillis = millis();

    // Print status every 10 seconds
    if (currentMillis - lastPrint >= 10000) {
        lastPrint = currentMillis;

        Serial.printf("Uptime: %lu seconds | Free Heap: %d bytes | WiFi: %s",
                      currentMillis / 1000,
                      ESP.getFreeHeap(),
                      wifiProv.isConnected() ? "Connected" : "Disconnected");

        if (wifiProv.isConnected()) {
            Serial.printf(" | IP: %s | RSSI: %d dBm",
                         wifiProv.getIP().c_str(),
                         wifiProv.getRSSI());
        }
        Serial.println();
    }

    delay(10);
}

void printSystemInfo() {
    Serial.println("System Information:");
    Serial.printf("  Chip Model: %s\n", ESP.getChipModel());
    Serial.printf("  Chip Revision: %d\n", ESP.getChipRevision());
    Serial.printf("  CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("  Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("  Flash Size: %d bytes\n", ESP.getFlashChipSize());
    Serial.printf("  SDK Version: %s\n\n", ESP.getSdkVersion());
}

void handleSerialCommands() {
    static String commandBuffer = "";

    while (Serial.available() > 0) {
        char c = Serial.read();

        if (c == '\n' || c == '\r') {
            if (commandBuffer.length() > 0) {
                // Trim whitespace
                commandBuffer.trim();
                commandBuffer.toLowerCase();

                // Process command
                if (commandBuffer == "reset wifi") {
                    Serial.println("\nResetting WiFi credentials...");
                    wifiProv.reset();
                    // Device will restart after reset
                } else if (commandBuffer == "help") {
                    Serial.println("\nAvailable commands:");
                    Serial.println("  reset wifi - Clear saved WiFi credentials and restart");
                    Serial.println("  help       - Show this help message");
                } else {
                    Serial.printf("\nUnknown command: %s\n", commandBuffer.c_str());
                    Serial.println("Type 'help' for available commands");
                }

                commandBuffer = "";
            }
        } else {
            commandBuffer += c;
        }
    }
}

