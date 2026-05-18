#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

const int p0 = A0;
const int p1 = D2;
const int p2 = D3;
const int p3 = D4;
const int p4 = D5;
const int p5 = D10;

const int a0 = -200;
const int a1 = 255;

const int b0 = 120;
const int b1 = 70;
const int b2 = 70;
const int b3 = 120;

const int c0 = 90;
const int c1[8] = {100, 125, 145, 165, 15, 35, 55, 80};

const int d0 = 200;
const unsigned long d1 = 50;

int d2 = 0;
int d3 = 4095;
unsigned long d4 = 0;

Servo s0, s1, s2;

int s3 = b0;
int s4 = b0;

int s5 = b2;
int s6 = b2;

int s7 = c0;
int s8 = c0;

unsigned long t0 = 0;
unsigned long t1 = 0;

int m0 = 0;

String u0 = "";

bool v0 = false;
uint32_t v1 = 0;

const char* w0 = "MAKERSPACE";
const char* w1 = "***";

AsyncWebServer w2(80);
AsyncWebSocket w3("/ws");

const char w4[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head><title>ESP32 Marble Machine</title></head>
<body>
  <h3>ESP32 Marble Machine</h3>
  <p id="x">Connecting...</p>
  <script>
    let y = new WebSocket(`ws://${window.location.hostname}/ws`);
    y.onmessage = e => document.getElementById('x').innerText = e.data;
  </script>
</body>
</html>
)rawliteral";

void f0(int x) {
  x = constrain(x, -a1, a1);

  m0 = x;

  if (x > 0) {
    ledcWrite(p4, x);
    ledcWrite(p5, 0);

  } else if (x < 0) {
    ledcWrite(p4, 0);
    ledcWrite(p5, -x);

  } else {
    ledcWrite(p4, 0);
    ledcWrite(p5, 0);
  }
}

void f1() {
  long x = 0;

  for (int i = 0; i < 20; i++) {
    x += analogRead(p0);
    delay(10);
  }

  d2 = x / 20;
  d3 = d2;
}

void f2() {
  int x = analogRead(p0);

  if (x < d3) d3 = x;
  else d3 += (d2 - d3) / 8;

  int y = d2 - x;

  if (y > d0 && millis() - d4 > d1) {
    d4 = millis();

    f0(0);

    w3.textAll("marble");
  }
}

void f3() {
  if (millis() - t0 < 15) return;

  t0 = millis();

  if (s3 < s4) {
    s3++;
    s0.write(s3);

  } else if (s3 > s4) {
    s3--;
    s0.write(s3);
  }

  if (s5 < s6) {
    s5++;
    s1.write(s5);

  } else if (s5 > s6) {
    s5--;
    s1.write(s5);
  }

  if (s7 < s8) {
    s7++;
    s2.write(s7);

  } else if (s7 > s8) {
    s7--;
    s2.write(s7);
  }
}

bool f4() {
  return s3 != s4 || s5 != s6 || s7 != s8;
}

void f5() {
  if (millis() - t1 < 5000) return;

  t1 = millis();

  Serial.printf(
    "%d %d %d %d\n",
    s3,
    s5,
    s7,
    m0
  );
}

void f6(const String& x0, bool x1, uint32_t x2) {
  String x3 = x0;

  x3.trim();

  if (!x3.length()) return;

  String x4 = x3;

  x4.toUpperCase();

  if (x4 == "C") {
    f1();

    if (x1) {
      v0 = true;
      v1 = x2;
    }

    return;
  }

  bool x5 = false;

  if (x4.indexOf("M:OFF") != -1) {
    f0(0);
    x5 = true;

  } else if (x4.indexOf("M:ON") != -1) {
    f0(a0);
    x5 = true;

  } else {
    int x6 = x4.indexOf("M:");

    if (x6 != -1) {
      int x7 = x4.indexOf(',', x6);

      String x8 =
        (x7 == -1)
          ? x4.substring(x6 + 2)
          : x4.substring(x6 + 2, x7);

      x8.trim();

      if (
        x8.length() &&
        (isDigit(x8.charAt(0)) || x8.charAt(0) == '-')
      ) {
        f0(x8.toInt());
        x5 = true;
      }
    }
  }

  if (x4.indexOf("H:OPEN") != -1) {
    s4 = b1;
    s6 = b3;
    x5 = true;

  } else if (x4.indexOf("H:CLOSE") != -1) {
    s4 = b0;
    s6 = b2;
    x5 = true;
  }

  int x9 = x4.indexOf("T:");

  if (x9 != -1) {
    int y0 = x4.indexOf(',', x9);

    String y1 =
      (y0 == -1)
        ? x4.substring(x9 + 2)
        : x4.substring(x9 + 2, y0);

    y1.trim();

    if (y1 == "HOME") {
      s8 = c0;
      x5 = true;

    } else {
      int y2 = y1.toInt();

      if (y2 >= 1 && y2 <= 8) {
        s8 = c1[y2 - 1];
        x5 = true;
      }
    }
  }

  if (x1 && x5) {
    v0 = true;
    v1 = x2;
  }
}

void f7(
  AsyncWebSocket* x0,
  AsyncWebSocketClient* x1,
  AwsEventType x2,
  void* x3,
  uint8_t* x4,
  size_t x5
) {
  switch (x2) {

    case WS_EVT_CONNECT:
      x1->text("ready");
      break;

    case WS_EVT_DISCONNECT:
      if (x1->id() == v1) {
        v0 = false;
        v1 = 0;
      }
      break;

    case WS_EVT_DATA: {
      AwsFrameInfo* x6 = (AwsFrameInfo*)x3;

      if (
        x6->final &&
        x6->index == 0 &&
        x6->len == x5 &&
        x6->opcode == WS_TEXT
      ) {
        x4[x5] = 0;

        String x7 = (char*)x4;

        x7.trim();

        f6(x7, true, x1->id());
      }

      break;
    }

    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);

  delay(1000);

  pinMode(p0, INPUT);
  pinMode(p4, OUTPUT);
  pinMode(p5, OUTPUT);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);

  s0.setPeriodHertz(50);
  s1.setPeriodHertz(50);
  s2.setPeriodHertz(50);

  s0.attach(p1, 500, 2400);
  s1.attach(p2, 500, 2400);
  s2.attach(p3, 500, 2400);

  s0.write(b0);
  s1.write(b2);
  s2.write(c0);

  ledcAttach(p4, 1000, 8);
  ledcAttach(p5, 1000, 8);

  f0(0);

  WiFi.mode(WIFI_STA);
  WiFi.begin(w0, w1);

  int x = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);

    x++;

    if (x > 30) {
      delay(5000);
      ESP.restart();
    }
  }

  w3.onEvent(f7);

  w2.addHandler(&w3);

  w2.on("/", HTTP_GET, [](AsyncWebServerRequest* x) {
    x->send_P(200, "text/html", w4);
  });

  w2.begin();

  f1();
}

void loop() {
  f2();

  f3();

  f5();

  while (Serial.available()) {
    char x = Serial.read();

    if (x == '\n' || x == '\r') {

      if (u0.length()) {
        f6(u0, false, 0);
        u0 = "";
      }

    } else {
      u0 += x;
    }
  }

  if (v0 && !f4()) {
    w3.textAll("done");

    v0 = false;
    v1 = 0;
  }

  w3.cleanupClients();
}
