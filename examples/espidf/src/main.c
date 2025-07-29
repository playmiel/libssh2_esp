/**
 * @file main.c
 * @brief libssh2_esp ESP-IDF Example
 * 
 * This example demonstrates how to use libssh2_esp with ESP-IDF framework
 * to establish an SSH connection and execute a simple command.
 * The same libssh2_esp library works with both Arduino and ESP-IDF!
 * 
 * @author playmiel
 * @license BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include <libssh2_esp.h>

static const char *TAG = "libssh2_esp_example";

// WiFi credentials - CHANGE THESE!
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// SSH server credentials - CHANGE THESE!
#define SSH_HOSTNAME "test.rebex.net"  // Public SSH test server
#define SSH_PORT 22
#define SSH_USERNAME "demo"
#define SSH_PASSWORD "password"

// Function prototypes
void wifi_init_sta(void);
void demonstrate_ssh(void);
void execute_command(LIBSSH2_SESSION *session, const char* command);
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

static bool wifi_connected = false;

void app_main(void)
{
    ESP_LOGI(TAG, "\n=== libssh2_esp ESP-IDF Example ===");
    ESP_LOGI(TAG, "Framework: %s", libssh2_esp_get_framework());
    
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // Initialize WiFi
    wifi_init_sta();
    
    // Wait for WiFi connection
    while (!wifi_connected) {
        ESP_LOGI(TAG, "Waiting for WiFi connection...");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    // Initialize libssh2_esp
    ESP_LOGI(TAG, "Initializing libssh2_esp...");
    if (libssh2_esp_init() != 0) {
        ESP_LOGE(TAG, "Failed to initialize libssh2_esp");
        return;
    }
    
    // Demonstrate SSH connection
    demonstrate_ssh();
    
    // Main loop
    while (1) {
        ESP_LOGI(TAG, "Example running... (SSH demo completed in setup)");
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "Retry to connect to the AP");
        wifi_connected = false;
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "WiFi connected! IP: " IPSTR, IP2STR(&event->ip_info.ip));
        wifi_connected = true;
    }
}

void wifi_init_sta(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi initialization finished.");
}

void demonstrate_ssh(void)
{
    ESP_LOGI(TAG, "\n=== SSH Connection Demo ===");
    
    // Create socket connection
    ESP_LOGI(TAG, "Connecting to %s:%d...", SSH_HOSTNAME, SSH_PORT);
    int sock = libssh2_esp_socket_connect(SSH_HOSTNAME, SSH_PORT);
    if (sock < 0) {
        ESP_LOGE(TAG, "Failed to connect to SSH server");
        return;
    }
    
    ESP_LOGI(TAG, "Socket connected successfully (fd: %d)", sock);
    
    // Initialize SSH session
    LIBSSH2_SESSION *session = libssh2_session_init();
    if (!session) {
        ESP_LOGE(TAG, "Failed to create SSH session");
        libssh2_esp_socket_close(sock);
        return;
    }
    
    // Set session to non-blocking mode
    libssh2_session_set_blocking(session, 0);
    
    // Perform SSH handshake
    ESP_LOGI(TAG, "Performing SSH handshake...");
    int rc;
    while ((rc = libssh2_session_handshake(session, sock)) == LIBSSH2_ERROR_EAGAIN) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    
    if (rc) {
        ESP_LOGE(TAG, "SSH handshake failed with error %d", rc);
        libssh2_session_free(session);
        libssh2_esp_socket_close(sock);
        return;
    }
    
    ESP_LOGI(TAG, "SSH handshake successful!");
    
    // Authenticate
    ESP_LOGI(TAG, "Authenticating...");
    while ((rc = libssh2_userauth_password(session, SSH_USERNAME, SSH_PASSWORD)) == LIBSSH2_ERROR_EAGAIN) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    
    if (rc) {
        ESP_LOGE(TAG, "Authentication failed with error %d", rc);
        libssh2_session_disconnect(session, "Authentication failed");
        libssh2_session_free(session);
        libssh2_esp_socket_close(sock);
        return;
    }
    
    ESP_LOGI(TAG, "Authentication successful!");
    
    // Execute simple commands
    execute_command(session, "echo 'Hello from ESP32 via SSH!'");
    execute_command(session, "pwd");
    execute_command(session, "ls -la");
    
    // Cleanup
    ESP_LOGI(TAG, "Cleaning up SSH connection...");
    libssh2_session_disconnect(session, "Normal shutdown");
    libssh2_session_free(session);
    libssh2_esp_socket_close(sock);
    
    ESP_LOGI(TAG, "SSH demo completed successfully!");
}

void execute_command(LIBSSH2_SESSION *session, const char* command)
{
    ESP_LOGI(TAG, "\nExecuting command: %s", command);
    
    LIBSSH2_CHANNEL *channel;
    int rc;
    
    // Open channel
    while ((channel = libssh2_channel_open_session(session)) == NULL && 
           libssh2_session_last_error(session, NULL, NULL, 0) == LIBSSH2_ERROR_EAGAIN) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    
    if (!channel) {
        ESP_LOGE(TAG, "Failed to open channel");
        return;
    }
    
    // Execute command
    while ((rc = libssh2_channel_exec(channel, command)) == LIBSSH2_ERROR_EAGAIN) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    
    if (rc) {
        ESP_LOGE(TAG, "Failed to execute command, error %d", rc);
        libssh2_channel_free(channel);
        return;
    }
    
    // Read output
    char buffer[1024];
    int bytes_read;
    
    ESP_LOGI(TAG, "Command output:");
    while (true) {
        bytes_read = libssh2_channel_read(channel, buffer, sizeof(buffer) - 1);
        if (bytes_read == LIBSSH2_ERROR_EAGAIN) {
            vTaskDelay(10 / portTICK_PERIOD_MS);
            continue;
        }
        if (bytes_read <= 0) break;
        
        buffer[bytes_read] = '\0';
        printf("%s", buffer);  // Use printf for direct output
    }
    
    // Close channel
    while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    
    libssh2_channel_free(channel);
    ESP_LOGI(TAG, "Command completed.");
}