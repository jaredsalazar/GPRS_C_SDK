#ifndef A9G_ARDUINO_NETWORK_H
#define A9G_ARDUINO_NETWORK_H

#include <stddef.h>

#include "WString.h"

typedef enum{
    NETWORK_STATUS_OFFLINE = 0,
    NETWORK_STATUS_REGISTERING,
    NETWORK_STATUS_REGISTERED,
    NETWORK_STATUS_DETACHED,
    NETWORK_STATUS_ATTACHING,
    NETWORK_STATUS_ATTACHED,
    NETWORK_STATUS_DEACTIVED,
    NETWORK_STATUS_ACTIVATING,
    NETWORK_STATUS_ACTIVATED,
    NETWORK_STATUS_ATTACH_FAILED,
    NETWORK_STATUS_ACTIVATE_FAILED,
    NETWORK_STATUS_MAX
} Network_Status_t;

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
