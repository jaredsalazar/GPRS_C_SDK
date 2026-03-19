#ifndef A9G_ARDUINO_NETWORK_H
#define A9G_ARDUINO_NETWORK_H

#include <stdbool.h>
#include <stddef.h>

#include "WString.h"
#include "api_inc_network.h"

class A9GNetworkClass {
public:
    bool begin(const char* apn, const char* userName = "", const char* userPasswd = "");
    bool attach();
    bool activate();

    bool registered() const;
    bool attached() const;
    bool connected() const;

    bool getIP(char* ip, size_t size) const;
    String ip() const;
    int signalQuality() const;
    Network_Status_t status() const;
};

extern A9GNetworkClass Cellular;

#endif
