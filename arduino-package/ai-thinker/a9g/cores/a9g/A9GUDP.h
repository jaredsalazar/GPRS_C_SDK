#ifndef A9G_ARDUINO_A9G_UDP_H
#define A9G_ARDUINO_A9G_UDP_H

#include "UDP.h"

class A9GUDP : public UDP {
public:
    A9GUDP();

    virtual size_t write(unsigned char value);
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();

    virtual unsigned char begin(unsigned int port);
    virtual int beginPacket(IPAddress ip, unsigned int port);
    virtual int beginPacket(const char* host, unsigned int port);
    virtual int endPacket();
    virtual void stop();

private:
    int _socket;
    int _peek;
    bool _connected;
    unsigned int _localPort;
    unsigned int _remotePort;
    IPAddress _remoteIp;
    unsigned char _txBuffer[256];
    size_t _txLength;

    int readIntoPeek();
};

#endif
