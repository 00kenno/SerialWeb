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
#include <Stream.h>

#if defined(ESP32) || defined(LIBRETINY)
#include <AsyncTCP.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
#include <RPAsyncTCP.h>
#include <WiFi.h>
#include <AsyncUDP_RP2040W.h>
#include <hardware/sync.h>
#endif
#include <ESPAsyncWebServer.h>

namespace SWNamespace {

  class SWLock {
    public:
      SWLock()
#if defined(ESP32) || defined(LIBRETINY)
        : _mux(portMUX_INITIALIZER_UNLOCKED)
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
        : _lock(nullptr), _saved_irq(0), _initialized(false)
#endif
      {}

      ~SWLock() {
#if defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
        if (_initialized && _lock) {
          int spin_num = spin_lock_get_num(_lock);
          spin_lock_unclaim(spin_num);
        }
#endif
      }

      void lock() {
#if defined(ESP32) || defined(LIBRETINY)
        if (xPortInIsrContext()) {
          portENTER_CRITICAL_ISR(&_mux);
        } else {
          portENTER_CRITICAL(&_mux);
        }
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
        if (!_initialized) {
          int spin_num = spin_lock_claim_unused(false);
          if (spin_num >= 0) {
            _lock = spin_lock_init(spin_num);
          }
          _initialized = true;
        }
        if (_lock) {
          _saved_irq = spin_lock_blocking(_lock);
        }
#else
        noInterrupts();
#endif
      }

      void unlock() {
#if defined(ESP32) || defined(LIBRETINY)
        if (xPortInIsrContext()) {
          portEXIT_CRITICAL_ISR(&_mux);
        } else {
          portEXIT_CRITICAL(&_mux);
        }
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
        if (_lock) {
          spin_unlock(_lock, _saved_irq);
        }
#else
        interrupts();
#endif
      }

    private:
#if defined(ESP32) || defined(LIBRETINY)
      portMUX_TYPE _mux;
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
      spin_lock_t *_lock;
      uint32_t _saved_irq;
      volatile bool _initialized;
#endif
  };
  
  class SWClass : public Stream {
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
      
      int available() override;
      int read() override;
      int peek() override;

      // Config Setter
      void setMaxClients (uint16_t _maxClients);

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
      uint8_t rx_buffer[BUFFER_SIZE];
      volatile int rx_head;
      volatile int rx_tail;
      SWLock lock_obj;
      
      // Override `write()` in `Print.h`
      size_t write(uint8_t c) override;
      size_t write(const uint8_t *buffer, size_t size) override;
      void flush() override;
  };

  extern SWClass SerialWeb;

} // namespace SWNamespace

using namespace SWNamespace;
