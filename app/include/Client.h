#ifndef A9G_ARDUINO_CLIENT_H
#define A9G_ARDUINO_CLIENT_H

#include "IPAddress.h"
#include "Stream.h"

class Client : public Stream {
public:
    virtual int connect(IPAddress ip, uint16_t port) = 0;
    virtual int connect(const char* host, uint16_t port) = 0;
    virtual uint8_t connected() = 0;
    virtual void stop() = 0;
};

#endif
