#include <Arduino.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  int raw0 = analogRead(A0);
  int mv0 = analogReadMilliVolts(A0);
  int raw1 = analogRead(A1);
  int mv1 = analogReadMilliVolts(A1);

  Serial.print("A0 raw=");
  Serial.print(raw0);
  Serial.print(" mv=");
  Serial.print(mv0);
  Serial.print(" A1 raw=");
  Serial.print(raw1);
  Serial.print(" mv=");
  Serial.println(mv1);
  delay(1000);
}
