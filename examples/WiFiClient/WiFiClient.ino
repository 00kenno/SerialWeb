#include <SerialWeb.h>

constexpr char SSID[] = "your-ssid";
constexpr char PASSWORD[] = "your-password";

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  IPAddress localIP = WiFi.localIP();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(localIP);

  SerialWeb.begin(localIP);
}

void loop() {
  char label[] = "NOW_TIME";
  char value[32];

  // send関数で送信したデータは，ダッシュボードに表示されます．
  sprintf(value, "%ld", millis());
  SerialWeb.send(label, value);

  // print関数やprintln関数はSerialなどと同様に使用できます．
  SerialWeb.print("Hello, ");
  SerialWeb.println("world.");

  // 読み取りは，readString関数のみの実装です．
  // available関数はbool型です．
  if (SerialWeb.available()) {
    // Stringオブジェクトをprintfの%sに直接渡すとメモリ破壊や無表示の原因となるため、.c_str()を使用します。
    String msg = SerialWeb.readString();
    Serial.printf("readString: %s\n", msg.c_str());
  }

  delay(100);
}
