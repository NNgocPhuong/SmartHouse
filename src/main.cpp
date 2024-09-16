#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "../include/System/System.h"

const int JSON_LEN = 1024;
char jbuf[JSON_LEN] = { };

char Hex(int a) {
  if (a < 10) return char(a | '0');
  return char(a + 55);
}
class JBUF : public Buffer {
public:
  JBUF() : Buffer(jbuf, JSON_LEN) {}

public:
  JBUF& MAC() {
    uint8_t m[6];
    esp_read_mac(m, ESP_MAC_WIFI_STA);

    *this << '\"';
    for (int i = 0; i < 6; i++) {
      *this << Hex(m[i] >> 4) << Hex(m[i] & 15);
    }
    *this << '\"';
    return *this;
  }
  JBUF& Start() { Seek(0); return Open(); }
  JBUF& End() { return Close(); }
  JBUF& Open() { *this << '{'; return *this; }
  JBUF& Close() { *this << '}'; return *this; }
  JBUF& Comma() { *this << ','; return *this; }
  JBUF& Name(LPCHAR s) { *this << '\"' << s << '\"' << ':'; return *this; }
  JBUF& Value(LPCHAR s) { *this << '\"' << s << '\"'; return *this; }
} json;

// put function declarations here:
LPCHAR ssid = "Mit Nam";
LPCHAR password = "09082805";
LPCHAR serverName = "https://slock.vst.edu.vn/api/all";

HTTPClient http;
Log _log;
System _system;

class Running : public Counter {
  void on_interrupt() override {
    _log << "Stop" << endl;
  }
} running;

class RequestTimer : public Timer
{
public:
  RequestTimer() : Timer(5000){}
  
  void on_restart() override{
    json.Start()
      .Name("url").Value("connect/check").Comma()
      .Name("value")
        .Open()
          .Name("_id").MAC()
        .Close()
    .End();
      
    _log << LPCHAR(json) << endl;
    int httpCode = http.POST(LPCHAR(json));
    _log << httpCode << endl;
    running.Start(3000);
  }
} rqTimer;


void setup() {
  
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  _system.Reset();
  while (WiFi.status() == WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to Wifi");
  }
  
  _log << "Connected: " << WiFi.macAddress() << endl;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");

  //running.Start(3000);
}

void loop() {
  
  _system.Loop();
}

