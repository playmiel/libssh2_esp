/**
 * @file main.cpp
 * @brief libssh2_esp Arduino Example
 * 
 * This example demonstrates how to use libssh2_esp with Arduino framework
 * to establish an SSH connection and execute a simple command.
 * 
 * @author playmiel
 * @license BSD-3-Clause
 */

#include <Arduino.h>
#include <WiFi.h>
#include <libssh2_esp.h>

// WiFi credentials - CHANGE THESE!
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// SSH server credentials - CHANGE THESE!
const char* ssh_hostname = "test.rebex.net";  // Public SSH test server
const int ssh_port = 22;
const char* ssh_username = "demo";
const char* ssh_password = "password";
void demonstrateSSH();
void executeCommand(LIBSSH2_SESSION *session, const char* command);
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n=== libssh2_esp Arduino Example ===");
    Serial.printf("Framework: %s\n", libssh2_esp_get_framework());
    
    // Initialize WiFi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.printf("WiFi connected! IP: %s\n", WiFi.localIP().toString().c_str());
    
    // Initialize libssh2_esp
    Serial.println("Initializing libssh2_esp...");
    if (libssh2_esp_init() != 0) {
        Serial.println("ERROR: Failed to initialize libssh2_esp");
        return;
    }
    
    // Demonstrate SSH connection
    demonstrateSSH();
}

void loop() {
    // Main loop - nothing to do here for this example
    delay(10000);
    Serial.println("Example running... (check setup() for SSH demo)");
}

void demonstrateSSH() {
    Serial.println("\n=== SSH Connection Demo ===");
    
    // Create socket connection
    Serial.printf("Connecting to %s:%d...\n", ssh_hostname, ssh_port);
    int sock = libssh2_esp_socket_connect(ssh_hostname, ssh_port);
    if (sock < 0) {
        Serial.println("ERROR: Failed to connect to SSH server");
        return;
    }
    
    Serial.printf("Socket connected successfully (fd: %d)\n", sock);
    
    // Initialize SSH session
    LIBSSH2_SESSION *session = libssh2_session_init();
    if (!session) {
        Serial.println("ERROR: Failed to create SSH session");
        libssh2_esp_socket_close(sock);
        return;
    }
    
    // Set session to non-blocking mode
    libssh2_session_set_blocking(session, 0);
    
    // Perform SSH handshake
    Serial.println("Performing SSH handshake...");
    int rc;
    while ((rc = libssh2_session_handshake(session, sock)) == LIBSSH2_ERROR_EAGAIN) {
        delay(10);
    }
    
    if (rc) {
        Serial.printf("ERROR: SSH handshake failed with error %d\n", rc);
        libssh2_session_free(session);
        libssh2_esp_socket_close(sock);
        return;
    }
    
    Serial.println("SSH handshake successful!");
    
    // Authenticate
    Serial.println("Authenticating...");
    while ((rc = libssh2_userauth_password(session, ssh_username, ssh_password)) == LIBSSH2_ERROR_EAGAIN) {
        delay(10);
    }
    
    if (rc) {
        Serial.printf("ERROR: Authentication failed with error %d\n", rc);
        libssh2_session_disconnect(session, "Authentication failed");
        libssh2_session_free(session);
        libssh2_esp_socket_close(sock);
        return;
    }
    
    Serial.println("Authentication successful!");
    
    // Execute a simple command
    executeCommand(session, "echo 'Hello from ESP32 via SSH!'");
    executeCommand(session, "pwd");
    executeCommand(session, "ls -la");
    
    // Cleanup
    Serial.println("Cleaning up SSH connection...");
    libssh2_session_disconnect(session, "Normal shutdown");
    libssh2_session_free(session);
    libssh2_esp_socket_close(sock);
    
    Serial.println("SSH demo completed successfully!");
}

void executeCommand(LIBSSH2_SESSION *session, const char* command) {
    Serial.printf("\nExecuting command: %s\n", command);
    
    LIBSSH2_CHANNEL *channel;
    int rc;
    
    // Open channel
    while ((channel = libssh2_channel_open_session(session)) == NULL && 
           libssh2_session_last_error(session, NULL, NULL, 0) == LIBSSH2_ERROR_EAGAIN) {
        delay(10);
    }
    
    if (!channel) {
        Serial.println("ERROR: Failed to open channel");
        return;
    }
    
    // Execute command
    while ((rc = libssh2_channel_exec(channel, command)) == LIBSSH2_ERROR_EAGAIN) {
        delay(10);
    }
    
    if (rc) {
        Serial.printf("ERROR: Failed to execute command, error %d\n", rc);
        libssh2_channel_free(channel);
        return;
    }
    
    // Read output
    char buffer[1024];
    int bytes_read;
    
    Serial.println("Command output:");
    while (true) {
        bytes_read = libssh2_channel_read(channel, buffer, sizeof(buffer) - 1);
        if (bytes_read == LIBSSH2_ERROR_EAGAIN) {
            delay(10);
            continue;
        }
        if (bytes_read <= 0) break;
        
        buffer[bytes_read] = '\0';
        Serial.print(buffer);
    }
    
    // Close channel
    while ((rc = libssh2_channel_close(channel)) == LIBSSH2_ERROR_EAGAIN) {
        delay(10);
    }
    
    libssh2_channel_free(channel);
    Serial.println("\nCommand completed.");
}