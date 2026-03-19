#ifndef A9G_ARDUINO_HARDWARE_SERIAL_H
#define A9G_ARDUINO_HARDWARE_SERIAL_H

#include <stdint.h>
#include <stddef.h>

#include "api_hal_uart.h"
#include "Stream.h"

class HardwareSerial : public Stream {
public:
    explicit HardwareSerial(UART_Port_t port);

    bool begin(unsigned long baud);
    virtual size_t write(uint8_t value);
    size_t write(const uint8_t* buffer, size_t size);
    size_t write(const char* str);
    virtual int available();
    int availableForWrite();
    virtual uint32_t readBytes(uint8_t* buffer, uint32_t length, uint32_t timeoutMs = 0);
    virtual int read();
    int read(uint32_t timeoutMs);
    virtual int peek();
    virtual void flush();
    size_t print(const char* str);
    size_t print(char c);
    size_t print(int value);
    size_t print(unsigned int value);
    size_t print(long value);
    size_t print(unsigned long value);
    size_t println();
    size_t println(const char* str);
    size_t println(int value);
    size_t println(unsigned int value);
    size_t println(long value);
    size_t println(unsigned long value);

private:
    UART_Port_t _port;
    bool _isOpen;
    int _peek;

    UART_Baud_Rate_t mapBaud(unsigned long baud) const;
};

extern HardwareSerial Serial;
extern HardwareSerial Serial2;

#endif
