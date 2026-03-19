#ifndef A9G_ARDUINO_A9G_UDP_H
#define A9G_ARDUINO_A9G_UDP_H

#include "UDP.h"

class A9GUDP : public UDP {
public:
    A9GUDP();

    virtual size_t write(uint8_t value);
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();

    virtual uint8_t begin(uint16_t port);
    virtual int beginPacket(IPAddress ip, uint16_t port);
    virtual int beginPacket(const char* host, uint16_t port);
    virtual int endPacket();
    virtual void stop();

private:
    int _socket;
    int _peek;
    bool _connected;
    uint16_t _localPort;
    uint16_t _remotePort;
    IPAddress _remoteIp;
    uint8_t _txBuffer[256];
    size_t _txLength;

    int readIntoPeek();
};

#endif
