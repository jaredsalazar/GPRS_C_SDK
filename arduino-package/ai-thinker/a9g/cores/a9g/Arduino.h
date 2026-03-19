#ifndef A9G_ARDUINO_CORE_H
#define A9G_ARDUINO_CORE_H

#include "pins_arduino.h"
#include "Print.h"
#include "Stream.h"
#include "WString.h"
#include "IPAddress.h"
#include "Printable.h"
#include "Client.h"
#include "UDP.h"
#include "HardwareSerial.h"
#include "SPI.h"
#include "Wire.h"
#include "A9GGPS.h"
#include "A9GLocation.h"
#include "A9GClient.h"
#include "A9GUDP.h"
#include "A9GNetwork.h"

#ifndef HIGH
#define HIGH 0x1
#endif

#ifndef LOW
#define LOW  0x0
#endif

#ifndef INPUT
#define INPUT  0x0
#endif

#ifndef OUTPUT
#define OUTPUT 0x1
#endif

typedef unsigned char byte;
typedef unsigned int word;
typedef bool boolean;

static const unsigned char A0 = 100;
static const unsigned char A1 = 101;

void setup(void);
void loop(void);

void pinMode(unsigned char pin, unsigned char mode);
void digitalWrite(unsigned char pin, unsigned char value);
int digitalRead(unsigned char pin);
int analogRead(unsigned char pin);
int analogReadMilliVolts(unsigned char pin);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long millis(void);
unsigned long micros(void);
void yield(void);

extern HardwareSerial Serial;
extern HardwareSerial Serial2;

#endif
