<div align="center">

[![GitHub release](https://img.shields.io/github/release/playmiel/libssh2_esp?include_prereleases=&sort=semver&color=brightgreen)](https://github.com/playmiel/libssh2_esp/releases/)
[![License](https://img.shields.io/badge/License-BSD--3--Clause-brightgreen)](#license)
[![Framework](https://img.shields.io/badge/Framework-Arduino%20%7C%20ESP--IDF-blue)](#frameworks)

# libssh2_esp

> libssh2 for ESP32 with Arduino and ESP-IDF support

</div>

A port of [libssh2-1.11.1](https://github.com/libssh2/libssh2/tree/libssh2-1.11.1) for ESP32 platforms with **dual framework support**. This library works seamlessly with both **Arduino/PlatformIO** and **ESP-IDF** frameworks, automatically detecting the build environment and adapting accordingly.

## 🚀 Features

- ✅ **Dual Framework Support**: Arduino/PlatformIO and ESP-IDF
- ✅ **Automatic Framework Detection**: No manual configuration needed
- ✅ **Unified API**: Same code works in both frameworks
- ✅ **Complete SSH Functionality**: All libssh2 features available
- ✅ **mbedTLS Backend**: Secure cryptography with ESP32's built-in mbedTLS
- ✅ **Easy Integration**: Simple include and use
- ✅ **Comprehensive Examples**: Working examples for both frameworks

## 📋 Frameworks

### Arduino/PlatformIO
Perfect for Arduino IDE users and PlatformIO projects. Simply add as a library dependency.

### ESP-IDF
Native ESP-IDF component support for professional development environments.

## 📦 Installation

### For Arduino/PlatformIO Projects

1. **PlatformIO**: Add to your `platformio.ini`:
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    https://github.com/playmiel/libssh2_esp.git
```

2. **Arduino IDE**: Install via Library Manager or download as ZIP.

### For ESP-IDF Projects

1. **As ESP-IDF managed component**:
```bash
idf.py add-dependency playmiel/libssh2_esp
```

2. **As git submodule**:
```bash
# In your project's components/ directory
git submodule add https://github.com/playmiel/libssh2_esp.git
git submodule update --init --recursive
```

## 🔧 Usage

### Basic Example (Works in both frameworks)

```cpp
#include <libssh2_esp.h>

void setup() {  // or app_main() for ESP-IDF
    // Initialize libssh2_esp
    libssh2_esp_init();
    
    // Framework is automatically detected
    printf("Framework: %s\n", libssh2_esp_get_framework());
    
    // Connect to SSH server
    int sock = libssh2_esp_socket_connect("example.com", 22);
    
    // Use standard libssh2 API
    LIBSSH2_SESSION *session = libssh2_session_init();
    libssh2_session_handshake(session, sock);
    // ... rest of SSH operations
    
    // Cleanup
    libssh2_esp_cleanup();
}
```

### Framework-Specific Features

The library automatically adapts to your framework:

- **Arduino**: Uses `Serial.printf()` for logging, `WiFi` for network
- **ESP-IDF**: Uses `ESP_LOGI()` for logging, native ESP-IDF networking

## 📚 Examples

### Arduino Example
- **Location**: [`examples/arduino/`](examples/arduino/)
- **Features**: WiFi connection, SSH authentication, command execution
- **Build**: `pio run -e esp32dev`

### ESP-IDF Example
- **Location**: [`examples/espidf/`](examples/espidf/)
- **Features**: Native ESP-IDF WiFi, FreeRTOS tasks, SSH operations
- **Build**: `idf.py build`

### Legacy Example (ESP-IDF only)
- **Location**: [`examples/ssh2_exec/`](examples/ssh2_exec/)
- **Features**: Original example from upstream project

## ⚙️ Configuration

### Arduino/PlatformIO
Configuration is automatic. Optional build flags:
```ini
build_flags =
    -DLIBSSH2_MBEDTLS      ; Use mbedTLS (default)
    -DLIBSSH2_NO_ZLIB      ; Disable compression
    -DHAVE_LIBSSH2_H       ; Enable libssh2 features
```

### ESP-IDF
Configure via menuconfig:
```bash
idf.py menuconfig
# Navigate to: Component config > libssh2
```

Options available:
- **Cryptography engine**: mbedTLS (recommended)
- **Debug logging**: Enable/disable debug output
- **Compression**: Enable/disable zlib compression

## 🔍 Framework Detection

The library automatically detects your framework:

```cpp
// Check current framework
const char* framework = libssh2_esp_get_framework();
// Returns: "Arduino" or "ESP-IDF"

// Framework-specific code (if needed)
#ifdef LIBSSH2_ESP_ARDUINO
    // Arduino-specific code
#elif defined(LIBSSH2_ESP_IDF)
    // ESP-IDF specific code
#endif
```

## 🛠️ API Reference

### Core Functions
- `int libssh2_esp_init(void)` - Initialize library
- `void libssh2_esp_cleanup(void)` - Cleanup resources
- `const char* libssh2_esp_get_framework(void)` - Get framework name

### Network Helpers
- `int libssh2_esp_socket_connect(const char* hostname, int port)` - Create connection
- `void libssh2_esp_socket_close(int sock)` - Close connection

### Standard libssh2 API
All standard libssh2 functions are available. See [libssh2 documentation](https://libssh2.org/docs.html).

## 🔧 Build Requirements

### Arduino/PlatformIO
- ESP32 Arduino Core 2.0+
- PlatformIO Core 6.0+ (for PlatformIO)

### ESP-IDF
- ESP-IDF 4.4+
- CMake 3.16+

## 🐛 Troubleshooting

### Common Issues

1. **Compilation errors**: Ensure you have the correct framework version
2. **Network issues**: Check WiFi credentials in examples
3. **SSH authentication**: Verify server credentials and key formats

### Debug Logging

Enable debug logging to troubleshoot issues:

**Arduino**:
```cpp
// Debug output goes to Serial
Serial.setDebugOutput(true);
```

**ESP-IDF**:
```bash
# Enable debug logging in menuconfig
idf.py menuconfig
# Component config > Log output > Default log verbosity > Debug
```

## 🤝 Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Test with both Arduino and ESP-IDF
4. Submit a pull request

## 📄 License

Released under [BSD-3-Clause](/LICENSE) license.

## 🙏 Acknowledgments

- Original [libssh2](https://libssh2.org/) project
- [@skuodi](https://github.com/skuodi) for the initial ESP-IDF port
- ESP32 Arduino and ESP-IDF communities

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/playmiel/libssh2_esp/issues)
- **Discussions**: [GitHub Discussions](https://github.com/playmiel/libssh2_esp/discussions)
- **Documentation**: [Wiki](https://github.com/playmiel/libssh2_esp/wiki)