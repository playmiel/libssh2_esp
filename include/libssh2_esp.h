/**
 * @file libssh2_esp.h
 * @brief libssh2 ESP32 wrapper with Arduino and ESP-IDF framework support
 * 
 * This header provides a unified interface for libssh2 on ESP32 platforms,
 * automatically detecting and adapting to Arduino or ESP-IDF frameworks.
 * 
 * @author skuodi, playmiel
 * @license BSD-3-Clause
 */

#ifndef LIBSSH2_ESP_H
#define LIBSSH2_ESP_H

// Framework detection and configuration (must be included first)
#include "framework_detection.h"

#ifdef __cplusplus
extern "C" {
#endif

// Core libssh2 includes
#include "libssh2.h"
#include "libssh2_sftp.h"

/**
 * @brief Initialize libssh2 for ESP32
 * @return 0 on success, negative on error
 */
int libssh2_esp_init(void);

/**
 * @brief Cleanup libssh2 resources
 */
void libssh2_esp_cleanup(void);

/**
 * @brief Get framework information
 * @return String describing the current framework
 */
const char* libssh2_esp_get_framework(void);

/**
 * @brief Create a socket connection (framework-agnostic)
 * @param hostname Target hostname
 * @param port Target port
 * @return Socket descriptor or negative on error
 */
int libssh2_esp_socket_connect(const char* hostname, int port);

/**
 * @brief Close socket connection
 * @param sock Socket descriptor
 */
void libssh2_esp_socket_close(int sock);

#ifdef __cplusplus
}
#endif

#endif // LIBSSH2_ESP_H