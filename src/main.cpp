#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "../include/System/System.h"

int status = 0;
void set_status(int v);

void turn_on() {
  set_status(1);
}
void turn_off() {
  set_status(0);
}

const int JSON_LEN = 1024;
char jbuf[JSON_LEN] = { };

char Hex(int a) {
  if (a < 10) return char(a | '0');
  return char(a + 55);
}

class Response {
  int sPost, mPost;
  LPCHAR source;

  int find(int start = -1) {
    for (int i = start + 1; char c = source[i]; i++) {
      if (c == ':') return i;
    }
    return -1;
  }

public:
  Response(const String& s) : source(s.c_str()) { 
    sPost = mPost = -1;
    int i = find();
    if (i < 0) return;

    sPost = find(i);
    if (sPost < 0) return;

    mPost = find(sPost);
  }

  int Value(int i) const {

    if (i < 0) return 0;

    int a = 0;
    for (int k = i + 1; char c = source[k]; k++) {
      if (c == ' ' && a == 0) continue;
      if (c > '9' || '0' > c) break;
      a = (a << 1) + (a << 3) + (c & 15);
    }
    return a;
  }

  int Status() const { return Value(sPost); }
  int Mins() const { return Value(mPost); }
};

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

  LPCHAR CreateApiBody(LPCHAR action) {
    Start()
      .Name("url").Value(action).Comma()
      .Name("value")
        .Open()
          .Name("_id").MAC().Comma()
          .Name("s").Number(status)
        .Close()
    .End();

    return ToString();
  }

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
    //tắt máy bơm và gửi trạng thái cho server
    turn_off();
  }

public:
  static int post(LPCHAR action) {
    _log << "--- post " << action << " ---" << endl;
    return http.POST(json.CreateApiBody(action));
  }
} may_bom;

class RequestTimer : public SecondCounter
{
public:  
  void on_one_second() override{   
    const int timeout = 3;
    _log << "Send checking in " << (timeout - Value()) << " seconds" << endl;

    if (Value() == timeout) {
      Reset();
 
      int httpCode = Running::post("connect/check");
      if (httpCode != 200) {
        _log << "Server error" << endl;
        turn_off();
        return;
      }

      String body = http.getString();
      Response res(body);

      _log << body << endl;

      int s = res.Status() & 1;
      if (s != status) {
        set_status(s);
        if (s) {
          int m = res.Mins();
          may_bom.Start(m * 60000);
        }
      }

      return;
    }
  }
} rqTimer;

void set_status(int v) {
  if (status != v) {
    status = v;
    _log << "status = " << status << endl;
    Running::post("connect/changed");
  }
}

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

