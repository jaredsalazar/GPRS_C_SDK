#ifndef A9G_ARDUINO_IP_ADDRESS_H
#define A9G_ARDUINO_IP_ADDRESS_H

#include <stdint.h>

#include "WString.h"

class IPAddress {
public:
    IPAddress();
    IPAddress(unsigned char first, unsigned char second, unsigned char third, unsigned char fourth);
    explicit IPAddress(const unsigned char* address);

    unsigned char operator[](int index) const;
    unsigned char& operator[](int index);

    bool fromString(const char* address);
    bool fromString(const String& address);
    String toString() const;

    bool operator==(const IPAddress& other) const;
    bool operator!=(const IPAddress& other) const;

private:
    unsigned char _address[4];
};

#endif
