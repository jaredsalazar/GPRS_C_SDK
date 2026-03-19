#ifndef A9G_ARDUINO_CORE_HARDWARE_SERIAL_H
#define A9G_ARDUINO_CORE_HARDWARE_SERIAL_H

#include <stddef.h>

#include "Stream.h"

class HardwareSerial : public Stream {
public:
    bool begin(unsigned long baud);
    virtual size_t write(unsigned char value);
    size_t write(const unsigned char* buffer, size_t size);
    size_t write(const char* str);
    virtual int available();
    int availableForWrite();
    virtual int read();
    int read(unsigned long timeoutMs);
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
};

#endif
