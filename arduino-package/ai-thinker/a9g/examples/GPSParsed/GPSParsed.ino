#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  GPS.begin();
  Location.setUpdateInterval(1000);
}

void loop() {
  Serial.print("valid=");
  Serial.print(Location.valid() ? 1 : 0);
  Serial.print(" fix=");
  Serial.print(Location.fixType());
  Serial.print(" sats=");
  Serial.print(Location.satellites());
  Serial.print(" lat=");
  Serial.print((long)(Location.latitude() * 1000000.0f));
  Serial.print(" lon=");
  Serial.print((long)(Location.longitude() * 1000000.0f));
  Serial.print(" alt=");
  Serial.print((long)(Location.altitude() * 100.0f));
  Serial.print(" speed_kph=");
  Serial.println((long)(Location.speedKph() * 100.0f));
  delay(1000);
}
