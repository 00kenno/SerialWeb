#include <Arduino.h>
#include "SerialWeb.h"

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

#ifdef TORICA
  #include "assets/TORICA_index.h"
#else
  #include "assets/index.h"
#endif

  AsyncWebServer SWClass::server(80); // HTTPサーバーインスタンスの定義
  AsyncWebSocket SWClass::ws("/ws"); // WebSocketエンドポイントの設定
  AsyncUDP SWClass::udp;

  SWClass::SWClass () {
    rx_buffer1[0] = '\0';
    rx_buffer2[0] = '\0';
    writing = rx_buffer1;
    reading = rx_buffer2;
    isReading = false;
    newData = false;
  }


  SWClass::~SWClass(){
    for (int i = 0; i < sizeof(labels)/sizeof(labels[0]); i++) {
      if (labels[i] != nullptr) {
        free(labels[i]); // strdup で確保したメモリを解放
        labels[i] = nullptr;
      }
    }
  }


  void SWClass::begin (const char *ssid, const char *password, 
    const IPAddress AP_IP, const IPAddress NET_MSK, const byte DNS_PORT)
  {
    WiFi.disconnect(true); // これがないとwebServerを再起動できない
    delay(1000); // この遅延は必須（これがないとwebServerが起動しない）
    WiFi.softAPConfig(AP_IP, AP_IP, NET_MSK); // IPアドレスの設定
    WiFi.softAP(ssid, password); // APモードでWiFi開始
    delay(500);  // IPアドレス取得のための遅延
    // Serial.print("AP IP address: ");
    // Serial.println(WiFi.softAPIP());
    
    begin(AP_IP, DNS_PORT);
  }


  void SWClass::begin (const IPAddress IP, const byte DNS_PORT) {
    // --- Async DNS Server (Captive Portal) Implementation ---
    if (udp.listen(DNS_PORT)) {
      udp.onPacket([IP](AsyncUDPPacket packet) {
        
        uint8_t* data = packet.data();
        size_t len = packet.length();
        
        // DNSヘッダー(12byte)以上のパケットで、かつ「クエリ(Flagsの最上位bitが0)」の場合のみ反応
        if (len > 12 && (data[2] & 0x80) == 0) {
          
          // --- レスポンスパケットの作成 ---
          AsyncUDPMessage response;

          // 1. Header (12 bytes)
          response.write(data[0]); // Transaction ID (MSB) - コピー
          response.write(data[1]); // Transaction ID (LSB) - コピー
          response.write(0x81);    // Flags: Response, Standard Query
          response.write(0x80);    // Flags: No Error, RA=1
          response.write(data[4]); // Questions (MSB) - コピー
          response.write(data[5]); // Questions (LSB) - コピー
          response.write(0x00);    // Answer RRs (MSB) -> 1個返す
          response.write(0x01);    // Answer RRs (LSB)
          response.write(0x00);    // Authority RRs
          response.write(0x00);    
          response.write(0x00);    // Additional RRs
          response.write(0x00);

          // 2. Question Section
          // 元のパケットの質問セクションをそのままコピーして送り返す
          // (ドメイン名 + Type(2) + Class(2))
          // 質問セクションの終わりを探す: 0x00 が出るまで
          size_t qPos = 12;
          while (qPos < len && data[qPos] != 0) {
            qPos++;
          }
          // 末尾の 0x00 と Type/Class (4bytes) を含める
          if (qPos + 5 <= len) {
             response.write(data + 12, (qPos + 5) - 12);
          } else {
             return; // パケット不正
          }

          // 3. Answer Section (A Record pointing to Self)
          // Name Pointer: 0xC00C (ヘッダ直後のドメイン名を参照)
          response.write(0xC0); 
          response.write(0x0C);
          
          // Type: A (Host Address) = 0x0001
          response.write(0x00); 
          response.write(0x01);
          
          // Class: IN (Internet) = 0x0001
          response.write(0x00); 
          response.write(0x01);
          
          // TTL (Time To Live): 60 seconds
          response.write(0x00); 
          response.write(0x00); 
          response.write(0x00); 
          response.write(0x3C);
          
          // Data Length: 4 bytes (IPv4)
          response.write(0x00); 
          response.write(0x04);
          
          // IP Address (自分のIP)
          response.write(IP[0]);
          response.write(IP[1]);
          response.write(IP[2]);
          response.write(IP[3]);

          // 返信
          packet.send(response);
        }
      });
    }
    // -----------------------------------------------------

    ws.onEvent(
      [this](AsyncWebSocket *server, AsyncWebSocketClient *client,
        AwsEventType type, void *arg, uint8_t *data, size_t len)
      {
        this->handleWsEvent(server, client, type, arg, data, len); // WebSocketイベントハンドラの登録
      }
    );
    // Serial.println("WebSocket server started.");

    // WebSocketの同時接続数を制限するミドルウェアの追加
    server.addHandler(&ws).addMiddleware(
      [this](AsyncWebServerRequest *request, ArMiddlewareNext next) {
        if (ws.count() > maxClients) { // ws.count()は現在のWSクライアントの数
          request->send(503, "text/plain", "Server is busy");
        }
        else {
          next(); // 次のミドルウェアと最後にハンドラを処理
        }
      }
    );
  
    server.on("/", HTTP_GET,
      [this](AsyncWebServerRequest *request) {this->handleRoot(request);}); // ルートハンドラの登録
    server.on("/generate_204", HTTP_GET,
      [this](AsyncWebServerRequest *request) {this->handleRoot(request);}); // Android Captive Portal対応
    server.on("/fwlink", HTTP_GET,
      [this](AsyncWebServerRequest *request) {this->handleRoot(request);}); // Windows Captive Portal対応
    server.onNotFound(
      [this](AsyncWebServerRequest *request) {this->handleRoot(request);}); // その他のリクエストはすべてルートへ誘導
    server.on("/send", HTTP_GET,
      [this](AsyncWebServerRequest *request) {this->receiveHttp(request);}); // iOSからはHTTPで受信

    server.addHandler(&ws); // WebSocketハンドラの追加

    server.begin(); // HTTPサーバーの開始
    // Serial.println("HTTP server started.");
  }
  

  void SWClass::send (const char *label, const char *value) {
    int dataIndex = -1;
    for (int i = 0; i < sizeof(labels)/sizeof(labels[0]); i++) {
      if (labels[i] == nullptr) { // 未登録のラベルを見つけた場合
        labels[i] = strdup(label); // ラベルをコピーして登録
        dataIndex = i; // データインデックスを設定
        // Serial.printf("Registered new label: %s at index %d\n", label, i);
        break; // ループを抜ける
      }
      else if (strcmp(labels[i], label) == 0) { // 既存のラベルを見つけた場合
        dataIndex = i; // 既存のラベルのインデックスを設定
        // Serial.printf("Found existing label: %s at index %d\n", label, i);
        break; // ループを抜ける
      }
    }
    if (dataIndex == -1) {
      // Serial.println("Error: No available slot for new label.");
      return; // ラベル登録に失敗した場合は終了
    }
    String msg = String("{\"index\":\"") + String(dataIndex) + String("\",\"label\":\"") + String(label) + String("\",\"value\":\"") + String(value) + String("\"}"); // JSON形式のメッセージ作成
    ws.textAll(msg); // すべてのクライアントにメッセージ送信
    ws.cleanupClients(maxClients); // クライアントのクリーンアップ
  }

  
  bool SWClass::available () {
    ws.cleanupClients(maxClients); // クライアントのクリーンアップ
    return newData;
  }


  String SWClass::readString () {
    isReading = true;
    char tempBuffer[BUFFER_SIZE];
    strcpy(tempBuffer, (char *)reading);
    newData = false;
    isReading = false;
    ws.cleanupClients(maxClients); // クライアントのクリーンアップ
    return String(tempBuffer);
  }


  void SWClass::setMaxClients (uint16_t _maxClients) {
    maxClients = _maxClients;
  }
  

  void SWClass::handleRoot (AsyncWebServerRequest *request) {
    request->send(200, "text/html", html); // HTMLコンテンツを返す
  }


  void SWClass::handleWsEvent (AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) { // WebSocketイベントハンドラ
    if (type == WS_EVT_CONNECT) { // クライアント接続時
      ws.textAll("new client connected"); // 全クライアントにメッセージ送信
      // Serial.println("ws connect");
      client->setCloseClientOnQueueFull(false); // キューが満杯でもクライアントを閉じない
      client->ping(); // 接続確認のpingを送信
    }
    else if (type == WS_EVT_DISCONNECT) { // クライアント切断時
      char msg[64];
      snprintf(msg, 64, "client disconnected. number of clients: %d", ws.count());
      ws.textAll(msg); // 全クライアントにメッセージ送信
      // Serial.println("ws disconnect");
    }
    else if (type == WS_EVT_ERROR) { // エラー発生時
      // Serial.println("ws error");
    }
    else if (type == WS_EVT_PONG) { // pong受信時
      // Serial.println("ws pong");
    }
    else if (type == WS_EVT_DATA) { // データ受信時
      AwsFrameInfo *info = (AwsFrameInfo *)arg; // フレーム情報の取得
      // Serial.printf("index: %" PRIu64 ", len: %" PRIu64 ", final: %" PRIu8 ", opcode: %" PRIu8 "\n", info->index, info->len, info->final, info->opcode); // フレーム情報の表示
      if (info->final && info->index == 0 && info->len == len) { // 完全なメッセージが1フレームで届いた場合
        if (info->opcode == WS_TEXT) { // テキストメッセージの場合
          // Serial.printf("WS Raw: %s\n", data);
          handleString((const char *)data, len);
        }        
      }
    }
  }


  void SWClass::receiveHttp (AsyncWebServerRequest *request) {
    if (request->hasParam("msg")) {
      String msg = request->getParam("msg")->value();
      // Serial.printf("HTTP Raw: %s\n", msg);

      handleString(msg.c_str(), msg.length());    
      request->send(200, "text/plain", "OK");
    }
    else {
      request->send(400, "text/plain", "Bad Request");
    }
  }


  void SWClass::handleString (const char *str, size_t len) {
    if (isReading) return; // データ落ちの可能性

    char *stash = (char *)reading;
    size_t cplen = min(len, (size_t)(BUFFER_SIZE - 1));
    memcpy((void *)writing, str, cplen);
    writing[cplen] = '\0';

    reading = writing;
    writing = stash;
    newData = true;
  }


  size_t SWClass::write (uint8_t c) {
    return write(&c, 1);
  }


  size_t SWClass::write (const uint8_t *buffer, size_t size) {
    String payload = "";
    payload.reserve(size); // メモリ確保
    for (size_t i = 0; i < size; i++) {
      char c = (char)buffer[i];

      switch (c) {
        case '"': {
          payload += "\\\""; break;
        }
        case '\\': {
          payload += "\\\\"; break;            
        }
        case '\n': {
          payload += "\\n"; break;
        }
        case '\r': {
          payload += "\\r"; break;
        }
        case '\t': {
          payload += "\\t"; break;
        }
        default: {
          if (c >= 0x20 || c < 0) {
            payload += c;
          }
          break;
        }
      }
    }

    String msg = String("{\"value\":\"") + payload + String("\"}"); // JSON形式のメッセージ作成
    ws.textAll(msg); // すべてのクライアントにメッセージ送信
    ws.cleanupClients(maxClients); // クライアントのクリーンアップ
    return size;
  }


  void SWClass::flush(){}


  SWClass SerialWeb; // グローバルインスタンスの作成

} // namespace SWNamespace
