#ifndef A9G_ARDUINO_CORE_STREAM_H
#define A9G_ARDUINO_CORE_STREAM_H

#include "Print.h"

class Stream : public Print {
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    int read(unsigned long timeoutMs);
    unsigned long readBytes(unsigned char* buffer, unsigned long length, unsigned long timeoutMs = 0);
    unsigned long readBytesUntil(char terminator, char* buffer, unsigned long length, unsigned long timeoutMs = 0);
    bool find(char target);
    bool find(const char* target);
    long parseInt(unsigned long timeoutMs = 0);
};

#endif
