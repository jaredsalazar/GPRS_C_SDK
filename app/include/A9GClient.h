#ifndef A9G_ARDUINO_A9G_CLIENT_H
#define A9G_ARDUINO_A9G_CLIENT_H

#include "Client.h"

class A9GClient : public Client {
public:
    A9GClient();

    virtual size_t write(uint8_t value);
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();

    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connect(const char* host, uint16_t port);
    virtual uint8_t connected();
    virtual void stop();
    virtual uint32_t readBytes(uint8_t* buffer, uint32_t length, uint32_t timeoutMs = 0);

private:
    int _socket;
    int _peek;
    bool _connected;

    int readIntoPeek();
};

#endif
