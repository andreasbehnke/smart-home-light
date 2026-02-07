# 4. Use NeoPixelBus for SK6812 RGBW LED Strip Control

Date: 2026-02-07

## Status

Accepted

## Context

The Smart Home Light Controller needs to control SK6812 RGBW LED strips connected to the ESP32. SK6812 is an addressable LED protocol similar to WS2812B but with an additional white channel (RGBW instead of RGB), requiring precise timing control.

Key requirements:
- Control SK6812 RGBW LED strips with full color and white channel support
- Efficient operation that doesn't interfere with WiFi connectivity
- Hardware-based timing to ensure reliable LED control
- Non-blocking operation to maintain web server responsiveness
- Support for various LED strip lengths and configurations

Common library options:
- **FastLED**: Popular but limited RGBW support, primarily designed for RGB LEDs
- **Adafruit NeoPixel**: Good RGBW support but uses software timing on ESP32
- **NeoPixelBus**: ESP32-optimized library using hardware RMT peripheral with full RGBW support

## Decision

We will use **NeoPixelBus by Makuna** (library: `makuna/NeoPixelBus`) for controlling SK6812 RGBW LED strips.

The implementation will:
- Use ESP32's RMT (Remote Control) peripheral for hardware-based timing
- Support full RGBW color control (Red, Green, Blue, White channels)
- Provide non-blocking operation that doesn't interfere with WiFi or web server
- Allow easy configuration of LED count and pin assignment
- Use the `NeoGrbwFeature` for SK6812 RGBW color order
- Use the `Neo800KbpsMethod` for SK6812 timing (800kHz data rate)

## Consequences

### Positive

- **Hardware-based timing**: Uses ESP32 RMT peripheral, ensuring precise timing without CPU overhead
- **Full RGBW support**: Native support for SK6812's white channel, not just RGB
- **Non-blocking**: Doesn't block other operations like WiFi, web server, or serial communication
- **ESP32 optimized**: Specifically designed for ESP32 architecture and peripherals
- **Better performance**: More efficient than software-based timing libraries
- **Multiple strip support**: Can control multiple LED strips independently using different RMT channels
- **Active maintenance**: Well-maintained library with regular updates and bug fixes
- **Flexible API**: Supports various LED types and configurations through template parameters
- **No WiFi interference**: Hardware timing prevents conflicts with WiFi operations

### Negative

- **ESP32 specific**: Less portable than platform-agnostic libraries like Adafruit NeoPixel
- **RMT channel limitation**: ESP32 has limited RMT channels (8 on most models), limiting simultaneous strips
- **Slightly more complex API**: Template-based API requires understanding of feature and method types
- **Library size**: ~70KB larger than minimal implementations
- **Learning curve**: Different API than popular FastLED library that some developers may be familiar with

## Implementation Notes

### Library Installation

Add to `platformio.ini`:
```ini
lib_deps =
    makuna/NeoPixelBus@^2.8.0
```

### Basic Usage Pattern

```cpp
#include <NeoPixelBus.h>

// Define LED strip parameters
#define LED_PIN 5
#define LED_COUNT 30

// Create strip object for SK6812 RGBW
// NeoGrbwFeature = GRBW color order (SK6812 uses GRB + W)
// Neo800KbpsMethod = 800kHz timing for SK6812
NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(LED_COUNT, LED_PIN);

void setup() {
    strip.Begin();
    strip.Show(); // Initialize all pixels to 'off'
}

void loop() {
    // Set pixel colors (R, G, B, W values 0-255)
    strip.SetPixelColor(0, RgbwColor(255, 0, 0, 0));  // Red
    strip.SetPixelColor(1, RgbwColor(0, 0, 0, 255));  // White
    strip.Show(); // Update the strip
}
```

### Configuration Considerations

- **Pin selection**: Any GPIO pin can be used (avoid pins 6-11 used for flash on some boards)
- **Power requirements**: SK6812 LEDs draw ~60mA per LED at full white, ensure adequate power supply
- **Data line**: May require level shifter (3.3V → 5V) for longer strips or reliable operation
- **Strip length**: Consider memory usage (~4 bytes per RGBW LED) and power requirements

### RMT Channel Management

ESP32 has 8 RMT channels. NeoPixelBus automatically allocates channels, but for multiple strips:
- Default allocation is automatic
- Can be specified if needed for advanced use cases
- Monitor available channels if using RMT for other purposes (IR, etc.)

## Alternatives Considered

### FastLED
- **Pros**: Popular, extensive documentation, rich feature set
- **Cons**: Limited RGBW support for SK6812, software timing on ESP32, can interfere with WiFi
- **Rejected**: Poor RGBW support makes it unsuitable for SK6812

### Adafruit NeoPixel
- **Pros**: Simple API, good RGBW support, widely used
- **Cons**: Software timing on ESP32, can interfere with WiFi, less efficient
- **Rejected**: Software timing causes WiFi interference and reduced efficiency

### Custom Implementation
- **Pros**: Full control, minimal dependencies
- **Cons**: Complex timing requirements, extensive testing needed, maintenance burden
- **Rejected**: Not worth the development and maintenance effort when excellent library exists

## References

- [NeoPixelBus GitHub Repository](https://github.com/Makuna/NeoPixelBus)
- [SK6812 Datasheet](https://cdn-shop.adafruit.com/product-files/2757/p2757_SK6812RGBW_REV01.pdf)
- [ESP32 RMT Peripheral Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/rmt.html)
