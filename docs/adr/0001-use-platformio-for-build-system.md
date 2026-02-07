# 1. Use PlatformIO as Build System

Date: 2026-02-07

## Status

Accepted

## Context

We need a build system and development environment for ESP32 firmware development. The project requires:
- Cross-platform compatibility (Linux, macOS, Windows)
- Library dependency management
- Support for multiple ESP32 board variants
- Easy IDE integration
- Professional development workflow

## Decision

We will use PlatformIO with the Arduino framework as our build system and development environment.

## Consequences

### Positive

- **Unified tooling**: Single tool for building, uploading, and monitoring across all platforms
- **Library management**: Built-in library dependency resolution and versioning
- **IDE integration**: Excellent VS Code integration with IntelliSense and debugging
- **Multi-board support**: Easy configuration for different ESP32 variants
- **Reproducible builds**: Lock file ensures consistent builds across team members
- **Active community**: Large user base and extensive documentation

### Negative

- **Learning curve**: Team members unfamiliar with PlatformIO need to learn the tool
- **Additional dependency**: Requires Python and PlatformIO installation
- **Build time**: Slightly slower initial builds compared to Arduino IDE
- **Configuration complexity**: More configuration options can be overwhelming initially

## Alternatives Considered

- **Arduino IDE**: Simple but limited scalability, poor library management, lacks professional features like proper build configuration and multi-environment support

- **ESP-IDF with CMake**: More control and better performance, but significantly steeper learning curve, more complex setup, and would slow down initial development

- **Arduino CLI**: Good for CI/CD but lacks IDE integration and user-friendly interface for development

PlatformIO was chosen as it provides the best balance between ease of use and professional features for this project's scope.
