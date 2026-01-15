# SerialWeb
キャプティブポータルをデバッグモニターとして用いるためのライブラリです．<br>
➔ デモページは[**こちら**](https://torica-org.github.io/SerialWeb/)

## ライブラリの導入
1. 上部の`Download ZIP`でZIPファイルをダウンロードしてください．
2. ArduinoIDEの `Sketch` -> `Include Library` -> `Add .ZIP Library...` で導入できます．もしくは，ArduinoIDEのライブラリが保存されたディレクトリ（Windowsでは通常`C:\Users\{UserName}\Documents\Arduino\libraries\`です．）に直接展開してください．

## 必要なライブラリ
以下のライブラリをライブラリマネージャからインストールしてください．
> [!NOTE]
> 名前の似たライブラリがたくさん存在します．作者の名前なども使って照合してください．

### 非同期通信機能(WebSocketなど)を提供するライブラリ
- name: `ESP Async WebServer`
- author: `ESP32Async`


### TCP通信管理機能を提供するライブラリ
- ESP32シリーズのマイコンの場合
  - name: `Async TCP`
  - author: `ESP32Async`
- Raspberry Pi Pico WなどのRP系マイコンの場合
  - name: `RPAsyncTCP`
  - author: `Hristo Gochkov,Khoi Hoang,Ayush Sharma`
  
> [!NOTE]
> Raspberry Pi Pico WなどのRP系マイコンの場合，非同期DNSサーバー機能のために以下のライブラリが必要です．
> - name: `AsyncUDP_RP2040W`
> - author: `Hristo Gochkov,Khoi Hoang`

## インスタンス
インスタンス化は不要です．グローバルインスタンスを提供しています．

## 関数
- `SerialWeb.begin(ssid, password)`/`SerialWeb.begin(ipAddress)`
  - 前者ではアクセスポイントモード，後者ではクライアントモードでWiFiが開始されます．

```cpp
constexpr char SSID[] = "SerialWeb";
constexpr char PASSWORD[] = "12345678";
void setup() {
  SerialWeb.begin(SSID, PASSWORD);
}
```
```cpp
void setup() {
  // WiFiアクセスポイントへの接続後
  IPAddress localIP = WiFi.localIP();
  SerialWeb.begin(localIP);
}
```

- `SerialWeb.send(label, value)`
  - Webページのダッシュボードにデータを表示します．`label`がすでに存在する場合，その`value`を更新します．

```cpp
char label[] = "NOW_TIME";
char value[32];
sprintf(value, "%ld", millis());
SerialWeb.send(label, value);
```

- `SerialWeb.print()`など
  - Webページのログに表示します．
  - `Print.h`を継承しています．`print()`や`println()`，`printf()`が使用可能です．

```cpp
SerialWeb.print("Hello, ");
SerialWeb.println("world."); // -> `Hello, world.`

SerialWeb.printf("Time: %ld", millis());
```

- `SerialWeb.readString`など
  - 実装が不完全です．使用することはおすすめしません．
