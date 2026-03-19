#ifndef A9G_ARDUINO_IP_ADDRESS_H
#define A9G_ARDUINO_IP_ADDRESS_H

#include <stdint.h>

#include "WString.h"

class IPAddress {
public:
    IPAddress();
    IPAddress(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);
    explicit IPAddress(const uint8_t* address);

    uint8_t operator[](int index) const;
    uint8_t& operator[](int index);

    bool fromString(const char* address);
    bool fromString(const String& address);
    String toString() const;

    bool operator==(const IPAddress& other) const;
    bool operator!=(const IPAddress& other) const;

private:
    uint8_t _address[4];
};

#endif
