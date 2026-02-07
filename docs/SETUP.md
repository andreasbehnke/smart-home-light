# Project Setup Guide

## Initial Setup

### 1. Install PlatformIO

**VS Code (Recommended):**
1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install "PlatformIO IDE" extension
3. Restart VS Code

**Command Line:**
```bash
pip install platformio
```

### 2. Configure Your Board

Copy the override template:
```bash
cp platformio_override.ini.example platformio_override.ini
```

Edit `platformio_override.ini` and set your board:

```ini
[platformio]
default_envs = wemos_d1_mini32    ; Your board here
```

**Available boards:**
- `wemos_d1_mini32` - Wemos D1 Mini ESP32 / Mini D2 ESP32
- `esp32dev` - Generic ESP32 DevKit
- `esp32-wroom-32` - ESP32-WROOM-32
- `esp32-s2` - ESP32-S2
- `esp32-s3` - ESP32-S3
- `esp32-c3` - ESP32-C3

### 3. Optional: Set Serial Port

If PlatformIO doesn't auto-detect your serial port, add to `platformio_override.ini`:

**Linux/Mac:**
```ini
upload_port = /dev/ttyUSB0
monitor_port = /dev/ttyUSB0
```

**Windows:**
```ini
upload_port = COM3
monitor_port = COM3
```

To find your port:
- **Linux/Mac:** `ls /dev/tty* | grep -i usb`
- **Windows:** Device Manager → Ports (COM & LPT)

## Build and Upload

### Using VS Code
1. Open PlatformIO sidebar
2. Click "Build" under PROJECT TASKS
3. Click "Upload" to flash firmware
4. Click "Monitor" to view serial output

### Using Command Line
```bash
# Build
pio run

# Upload
pio run --target upload

# Monitor
pio device monitor

# All in one
pio run --target upload && pio device monitor
```

## File Organization

### Configuration Files

- **platformio.ini** - Project configuration (committed to git)
  - Board definitions
  - Common build settings
  - Library dependencies

- **platformio_override.ini** - User-specific settings (NOT committed)
  - Board selection
  - Serial port configuration
  - Personal build preferences
  - Created from `platformio_override.ini.example`

### Why Use Override Files?

1. **Team Development:** Each developer can use different boards/ports
2. **Clean Git History:** No conflicts from personal preferences
3. **Easy Setup:** New developers copy the example and configure once
4. **Flexibility:** Override any setting without modifying main config

## Troubleshooting

### Serial Port Issues

**Linux - Permission Denied:**
```bash
sudo usermod -a -G dialout $USER
# Log out and back in
```

**Windows - Port Not Found:**
- Install CH340 or CP2102 USB drivers
- Check Device Manager for COM port number

### Upload Fails

1. Hold **BOOT** button while uploading
2. Reduce upload speed in `platformio_override.ini`:
   ```ini
   [env]
   upload_speed = 115200
   ```
3. Try different USB cable/port

### Build Errors

```bash
# Clean and rebuild
pio run --target clean
pio run
```

## Team Workflow

### For New Team Members

1. Clone repository
2. Install PlatformIO
3. Copy override template:
   ```bash
   cp platformio_override.ini.example platformio_override.ini
   ```
4. Edit `platformio_override.ini` with your board/port
5. Build and upload

### When Changing Boards

Just update `platformio_override.ini`:
```ini
[platformio]
default_envs = esp32-s3    ; Changed from wemos_d1_mini32
```

No need to modify tracked files!

## Advanced Configuration

### Custom Build Flags

Add to `platformio_override.ini`:
```ini
[env]
build_flags =
    ${common.build_flags}
    -DMY_DEBUG_MODE=1
    -DCUSTOM_FEATURE_ENABLED
```

### Multiple Environments

Build for different boards:
```bash
pio run -e wemos_d1_mini32
pio run -e esp32-s3
```

### Upload to Specific Port

```bash
pio run --target upload --upload-port /dev/ttyUSB1
```
