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
int FindValue(String s, int pos) {
    int val = 0;
    for (int i = 0; char c = s[i]; i++) {
        if (c == ':') {
            --pos;
            c = s[++i];
            continue;
        }
        if (pos == 0) {
            if (c < '0' || c > '9') break;
            val = val * 10 + int(c & 15);
        }
    }
    return val;
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
  JBUF& Number(int v) { *this << v; return *this; }

} json;

// put function declarations here:
LPCHAR ssid = "Mit Nam";
LPCHAR password = "09082805";
LPCHAR serverName = "https://slock.vst.edu.vn/api/all";
int status = 0;

HTTPClient http;
Log _log;
System _system;

class Running : public Counter {
  void on_interrupt() override {
    //tắt máy bơm và gửi trạng thái cho server
  }
} running;

class RequestTimer : public Timer
{
public:
  RequestTimer() : Timer(3000){}
  
  void on_restart() override{
    json.Start()
      .Name("url").Value("connect/check").Comma()
      .Name("value")
        .Open()
          .Name("_id").MAC()
        .Close()
    .End();
      
    
    int httpCode = http.POST(LPCHAR(json));
    int s = FindValue(http.getString(), 2);
    _log << s << endl;
    if((s & 1) != 0)
    {
      status = 1;
      // kích 1 chân relay lên
      running.Start(FindValue(http.getString(), 3) * 60000);
      json.Start()
      .Name("url").Value("connect/changed").Comma()
      .Name("value")
        .Open()
          .Name("_id").MAC().Comma()
          .Name("s").Number(status)
        .Close()
      .End();
    }
    else
    {
      status = 0;
    }

    //running.Start(3000);
    
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

