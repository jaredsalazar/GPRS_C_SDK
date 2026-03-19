#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  GPS.begin();
}

void loop() {
  while (GPS.available()) {
    Serial.write((uint8_t)GPS.read());
  }
  delay(50);
}
