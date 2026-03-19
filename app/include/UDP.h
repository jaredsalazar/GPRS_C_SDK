#ifndef A9G_ARDUINO_UDP_H
#define A9G_ARDUINO_UDP_H

#include "IPAddress.h"
#include "Stream.h"

class UDP : public Stream {
public:
    virtual uint8_t begin(uint16_t port) = 0;
    virtual int beginPacket(IPAddress ip, uint16_t port) = 0;
    virtual int beginPacket(const char* host, uint16_t port) = 0;
    virtual int endPacket() = 0;
    virtual void stop() = 0;
};

#endif
