#ifndef A9G_ARDUINO_UDP_H
#define A9G_ARDUINO_UDP_H

#include "IPAddress.h"
#include "Stream.h"

class UDP : public Stream {
public:
    virtual unsigned char begin(unsigned int port) = 0;
    virtual int beginPacket(IPAddress ip, unsigned int port) = 0;
    virtual int beginPacket(const char* host, unsigned int port) = 0;
    virtual int endPacket() = 0;
    virtual void stop() = 0;
};

#endif
