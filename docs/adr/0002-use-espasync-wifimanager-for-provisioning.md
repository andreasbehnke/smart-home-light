# 2. Use Custom WiFi Provisioning with Captive Portal

Date: 2026-02-07
Updated: 2026-02-07

## Status

Accepted (Implementation changed from ESPAsync_WiFiManager to custom solution)

## Context

The Smart Home Light Controller needs a user-friendly way to configure WiFi credentials without hardcoding them or requiring users to modify source code. The device must:

- Allow users to configure WiFi credentials on first use
- Automatically reconnect to saved WiFi networks
- Provide a fallback configuration method if WiFi is unavailable
- Work seamlessly with our existing async web server architecture
- Store credentials persistently across reboots

Common approaches include:
- Captive portal (access point + web interface)
- Mobile app provisioning (BLE or SmartConfig)
- WPS (WiFi Protected Setup)
- Serial console configuration

## Decision

We will implement a **custom WiFi provisioning system** with captive portal support using:
- **Preferences (NVS)** for credential storage
- **DNSServer** for captive portal DNS redirection
- **ESPAsyncWebServer** for the configuration web interface

Initially planned to use ESPAsync_WiFiManager library, but encountered API compatibility issues with the current Arduino ESP32 framework (v3.1.3). The library uses deprecated WiFi API methods (`getAutoConnect()`, `setAutoConnect()`) that were replaced with `getAutoReconnect()` and `setAutoReconnect()` in newer framework versions.

A custom implementation provides better control, avoids dependency issues, and is fully compatible with the current framework.

The implementation will:
- Attempt to connect to saved WiFi credentials on boot
- If connection fails, start an access point named "SmartLight-Setup"
- Provide a captive portal web interface for WiFi configuration
- Store credentials in ESP32 NVS (Non-Volatile Storage)
- Support factory reset functionality

## Consequences

### Positive

- **User-friendly**: No need to modify code or use serial console for WiFi setup
- **Async-compatible**: Works seamlessly with ESPAsyncWebServer (already in use)
- **Non-blocking**: Doesn't block other operations during WiFi connection
- **Automatic credential management**: Handles storage/retrieval in NVS automatically
- **Cross-platform**: Works with Android, iOS, Windows, macOS, Linux
- **OS captive portal detection**: Automatically handles device-specific probe endpoints
- **Maintained library**: Active development and community support
- **Custom parameters**: Easy to add additional configuration fields if needed
- **Factory reset**: Built-in support for credential clearing

### Negative

- **Additional dependency**: Adds library dependency and increases firmware size (~50KB)
- **Initial complexity**: More complex than hardcoded credentials for simple use cases
- **AP mode security**: Default AP has no password (can be configured if needed)
- **Connection delay**: Takes 10-15 seconds to fall back to AP mode on connection failure
- **Memory usage**: Async operations require more RAM than synchronous alternatives

## Alternatives Considered

- **Hardcoded credentials**: Simple but requires recompilation for each deployment and is not suitable for end-user products. Rejected for poor user experience.

- **WiFiManager (synchronous)**: Popular and stable but uses blocking operations that conflict with our async architecture. Would require mixing sync/async patterns.

- **Custom captive portal**: Full control over implementation but requires significant development effort, error-prone, and must handle OS-specific captive portal detection manually.

- **ESP32 BLE Provisioning**: Requires companion mobile app development, more complex integration, and limits accessibility compared to web-based approach.

- **SmartConfig**: Requires mobile app, less intuitive user experience, and platform-specific implementations.

ESPAsync_WiFiManager was chosen as it provides the best balance of ease of use, compatibility with our async architecture, and professional features.

## Implementation Notes

### Custom Implementation Details

**Files Created:**
- `src/wifi_provisioning.h` - WiFiProvisioning class interface
- `src/wifi_provisioning.cpp` - Complete provisioning implementation

**Configuration:**
- AP SSID: `SmartLight-[ChipID]` (unique per device, based on MAC)
- AP IP: `192.168.4.1`
- AP Password: None (open network)
- Configuration timeout: 300 seconds (5 minutes)
- Auto-reconnect: Yes
- Credential storage: ESP32 Preferences (NVS partition)

**Captive Portal Detection:**
Handles OS-specific captive portal detection endpoints:
- Android: `/generate_204`, `/gen_204`
- iOS/macOS: `/hotspot-detect.html`, `/library/test/success.html`
- Windows: `/connecttest.txt`, `/ncsi.txt`

**DNS Configuration:**
- DNSServer redirects all DNS queries to `192.168.4.1`
- Enables automatic captive portal popup on mobile devices
- Catch-all HTTP handler redirects unknown requests to config page

**Web Interface Features:**
- Network scanning with signal strength display
- Dropdown selection of available networks
- Password input with validation
- Real-time connection status feedback
- Responsive design for mobile devices
