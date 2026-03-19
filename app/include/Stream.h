#ifndef A9G_ARDUINO_STREAM_H
#define A9G_ARDUINO_STREAM_H

#include <stdint.h>

#include "Print.h"

class Stream : public Print {
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;

    virtual int read(uint32_t timeoutMs);
    virtual uint32_t readBytes(uint8_t* buffer, uint32_t length, uint32_t timeoutMs = 0);
    uint32_t readBytesUntil(char terminator, char* buffer, uint32_t length, uint32_t timeoutMs = 0);
    bool find(char target);
    bool find(const char* target);
    long parseInt(uint32_t timeoutMs = 0);
};

#endif
