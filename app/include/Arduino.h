#ifndef A9G_ARDUINO_H
#define A9G_ARDUINO_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "api_hal_gpio.h"
#include "api_hal_adc.h"
#include "api_hal_pm.h"
#include "api_os.h"
#include "api_debug.h"
#include "time.h"
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

#ifdef __cplusplus
extern "C" {
#endif

#define HIGH 0x1
#define LOW  0x0

#define INPUT  0x0
#define OUTPUT 0x1

#define LED_BUILTIN GPIO_PIN27
#define LED_STATUS  GPIO_PIN27
#define LED_UPLOAD  GPIO_PIN28
#define A0 100
#define A1 101

typedef uint8_t byte;
typedef unsigned int word;
typedef bool boolean;

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
int analogReadMilliVolts(uint8_t pin);
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);
unsigned long millis(void);
unsigned long micros(void);
void yield(void);

#ifdef __cplusplus
}

void setup(void);
void loop(void);
#endif

#endif
