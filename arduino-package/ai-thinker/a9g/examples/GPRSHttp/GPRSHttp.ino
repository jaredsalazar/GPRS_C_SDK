#include <Arduino.h>

static A9GClient client;
static bool requested = false;

void setup() {
  Serial.begin(115200);
  Cellular.begin("your_apn_here", "", "");
}

void loop() {
  if (!Cellular.connected()) {
    Serial.println("Waiting for GPRS...");
    delay(2000);
    return;
  }

  if (!requested) {
    Serial.print("IP: ");
    Serial.println(Cellular.ip());

    if (client.connect("example.com", 80)) {
      client.print("GET / HTTP/1.0\r\nHost: example.com\r\n\r\n");
      requested = true;
    } else {
      Serial.println("TCP connect failed");
      delay(3000);
    }
  }

  while (client.connected() && client.available()) {
    Serial.write((uint8_t)client.read());
  }

  delay(50);
}
