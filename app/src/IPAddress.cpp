#include "IPAddress.h"

static bool parseOctet(const char*& text, uint8_t* value)
{
    unsigned int octet = 0;
    bool hasDigit = false;

    while (*text >= '0' && *text <= '9')
    {
        hasDigit = true;
        octet = octet * 10 + (unsigned int)(*text - '0');
        if (octet > 255)
            return false;
        ++text;
    }

    if (!hasDigit)
        return false;

    *value = (uint8_t)octet;
    return true;
}

IPAddress::IPAddress()
{
    _address[0] = 0;
    _address[1] = 0;
    _address[2] = 0;
    _address[3] = 0;
}

IPAddress::IPAddress(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth)
{
    _address[0] = first;
    _address[1] = second;
    _address[2] = third;
    _address[3] = fourth;
}

IPAddress::IPAddress(const uint8_t* address)
{
    if (address == NULL)
    {
        _address[0] = 0;
        _address[1] = 0;
        _address[2] = 0;
        _address[3] = 0;
        return;
    }

    _address[0] = address[0];
    _address[1] = address[1];
    _address[2] = address[2];
    _address[3] = address[3];
}

uint8_t IPAddress::operator[](int index) const
{
    return _address[index];
}

uint8_t& IPAddress::operator[](int index)
{
    return _address[index];
}

bool IPAddress::fromString(const char* address)
{
    const char* text = address;
    uint8_t octets[4];
    int index = 0;

    if (text == NULL)
        return false;

    for (index = 0; index < 4; ++index)
    {
        if (!parseOctet(text, &octets[index]))
            return false;

        if (index < 3)
        {
            if (*text != '.')
                return false;
            ++text;
        }
    }

    if (*text != '\0')
        return false;

    _address[0] = octets[0];
    _address[1] = octets[1];
    _address[2] = octets[2];
    _address[3] = octets[3];
    return true;
}

bool IPAddress::fromString(const String& address)
{
    return fromString(address.c_str());
}

String IPAddress::toString() const
{
    return String((unsigned long)_address[0]) + "." +
           String((unsigned long)_address[1]) + "." +
           String((unsigned long)_address[2]) + "." +
           String((unsigned long)_address[3]);
}

bool IPAddress::operator==(const IPAddress& other) const
{
    return _address[0] == other._address[0] &&
           _address[1] == other._address[1] &&
           _address[2] == other._address[2] &&
           _address[3] == other._address[3];
}

bool IPAddress::operator!=(const IPAddress& other) const
{
    return !(*this == other);
}
