#include <Arduino.h>
#include "api_hal_watchdog.h"

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(500);
  Serial.println("A9G serial heartbeat start");
  Serial2.println("A9G serial2 heartbeat start");
}

void loop() {
  static unsigned long counter = 0;
  WatchDog_KeepAlive();
  Serial.print("tick ");
  Serial.println(counter);
  Serial2.print("tick ");
  Serial2.println(counter);
  counter++;
  delay(1000);
}
