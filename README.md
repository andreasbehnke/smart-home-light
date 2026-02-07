# Smart Home Light Controller

This lamp provides different light effects like evening ambient light or a wakeup light sequence. It is based on an ESP32 microcontroller controlling a LED strip with individual addressable LEDs. Different smart home alerts like open windows or open doors can be signaled by single LEDs.

## Project Setup

This project uses PlatformIO for ESP32 firmware development.

### Quick Start

1. **Install PlatformIO:** VS Code extension or `pip install platformio`
2. **Configure your board:** Copy `platformio_override.ini.example` to `platformio_override.ini` and set your board
3. **Build and upload:** `pio run --target upload`
4. **Monitor output:** `pio device monitor`

For detailed setup instructions, see [docs/SETUP.md](docs/SETUP.md).

## Project Structure

```
smart-home-light/
├── platformio.ini          # PlatformIO configuration & board selection
├── src/
│   └── main.cpp           # Main firmware (currently "Hello World")
├── include/               # Header files (future use)
├── lib/                   # Custom libraries (future use)
├── test/                  # Unit tests (future use)
└── data/                  # Filesystem data (future use)
```

## Current Status

The firmware currently implements a minimal "Hello World" application that:
- Initializes serial communication at 115200 baud
- Prints system information (CPU frequency, free heap, chip model)
- Outputs uptime and memory status every 5 seconds

This serves as the foundation for incremental development.

## Development Workflow

Features will be added incrementally with architecture decisions documented in [Architecture Decision Records (ADRs)](docs/adr/README.md).

The next steps will include:
1. LED strip control configuration
2. WiFi connectivity
3. Web server implementation
4. Light effects and modes
5. Smart home integration

## Troubleshooting

### Cannot find serial port
- Check USB cable connection
- Install USB-to-Serial drivers (CH340, CP2102, or FTAG drivers depending on your board)
- Linux: Add user to dialout group: `sudo usermod -a -G dialout $USER`

### Upload fails
- Press and hold BOOT button on ESP32 while uploading
- Try slower upload speed in platformio.ini: `upload_speed = 115200`
- Verify correct board selection

### Monitor shows garbage characters
- Check baud rate matches: `monitor_speed = 115200`
- Ensure proper USB connection

## License

See LICENSE file for details.
