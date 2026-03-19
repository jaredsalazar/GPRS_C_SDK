#include <Arduino.h>

static A9GUDP udp;

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

  if (udp.beginPacket("time.google.com", 123)) {
    udp.write((uint8_t)0x1B);
    udp.endPacket();
    Serial.println("UDP packet sent");
  } else {
    Serial.println("UDP beginPacket failed");
  }

  delay(5000);
}
