/**
 * @file SerialWeb.h
 * @brief SerialWeb Library for ESP32/RP2040
 * * Provides a web-based serial monitor interface via WebSocket.
 * * @author 00kenno
 * @date 2025
 * @license MIT
 * @copyright Copyright (c) 2025 00kenno
 */

#pragma once

#include <Arduino.h>
#include <Print.h>

#include <DNSServer.h>
#if defined(ESP32) || defined(LIBRETINY)
#include <AsyncTCP.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
#include <RPAsyncTCP.h>
#include <WiFi.h>
#include <AsyncUDP_RP2040W.h>
#endif
#include <ESPAsyncWebServer.h>

namespace SWNamespace {
  
  class SWClass : public Print {
    public:
      SWClass();
      ~SWClass();

      void begin (
        const char *ssid,
        const char *password,
        const IPAddress AP_IP = IPAddress(192, 168, 4, 1),
        const IPAddress NET_MSK = IPAddress(255, 255, 255, 0),
        const byte DNS_PORT = 53
      );
      void begin(const IPAddress IP, const byte DNS_PORT = 53);
      void send(const char *label, const char *value);
      bool available();
      String readString();

      // Config Setter
      void setMaxClients (uint16_t _maxClients);

      // Status Getter
      

    private:
      static AsyncWebServer server;
      static AsyncWebSocket ws;
      static AsyncUDP udp;
      
      void handleRoot(AsyncWebServerRequest *request);
      void handleWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
        AwsEventType type, void *arg, uint8_t *data, size_t len);
      void receiveHttp(AsyncWebServerRequest *request);
      void handleString(const char *str, size_t len);

      static constexpr int MAX_LABELS = 99;
      char *labels[MAX_LABELS] = {nullptr};

      uint16_t maxClients = DEFAULT_MAX_WS_CLIENTS;
      static constexpr int BUFFER_SIZE = 256;
      char rx_buffer1[BUFFER_SIZE];
      char rx_buffer2[BUFFER_SIZE];
      volatile char *writing;
      volatile char *reading;
      volatile bool isReading;
      volatile bool newData;
      
      // Override `write()` in `Print.h`
      size_t write(uint8_t c) override;
      size_t write(const uint8_t *buffer, size_t size) override;
      void flush() override;
  };

  extern SWClass SerialWeb;

} // namespace SWNamespace

using namespace SWNamespace;
