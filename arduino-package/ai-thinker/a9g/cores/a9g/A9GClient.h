#ifndef A9G_ARDUINO_A9G_CLIENT_H
#define A9G_ARDUINO_A9G_CLIENT_H

#include "Client.h"

class A9GClient : public Client {
public:
    A9GClient();

    virtual size_t write(unsigned char value);
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();

    virtual int connect(IPAddress ip, unsigned int port);
    virtual int connect(const char* host, unsigned int port);
    virtual unsigned char connected();
    virtual void stop();
    unsigned long readBytes(unsigned char* buffer, unsigned long length, unsigned long timeoutMs = 0);

private:
    int _socket;
    int _peek;
    bool _connected;
};

#endif
