#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n\n========================================");
    Serial.println("Smart Home Light Controller");
    Serial.println("========================================");
    Serial.println("Firmware initialized successfully!");
    Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("Chip Model: %s\n", ESP.getChipModel());
    Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
    Serial.println("========================================\n");
}

void loop() {
    static unsigned long lastPrint = 0;
    unsigned long currentMillis = millis();

    if (currentMillis - lastPrint >= 5000) {
        lastPrint = currentMillis;
        Serial.printf("Uptime: %lu seconds | Free Heap: %d bytes\n",
                      currentMillis / 1000, ESP.getFreeHeap());
    }

    delay(100);
}
