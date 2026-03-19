#include "Arduino.h"

void setup(void)
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("A9G Arduino compatibility layer");
}

void loop(void)
{
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED on");
    delay(1000);

    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED off");
    delay(1000);
}
