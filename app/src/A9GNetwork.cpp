#include "A9GNetwork.h"

#include "A9GRuntime.h"

A9GNetworkClass Cellular;

bool A9GNetworkClass::begin(const char* apn, const char* userName, const char* userPasswd)
{
    return A9G_NetworkBegin(apn, userName, userPasswd);
}

bool A9GNetworkClass::attach()
{
    return A9G_NetworkAttach();
}

bool A9GNetworkClass::activate()
{
    return A9G_NetworkActivate();
}

bool A9GNetworkClass::registered() const
{
    return A9G_NetworkIsRegistered();
}

bool A9GNetworkClass::attached() const
{
    return A9G_NetworkIsAttached();
}

bool A9GNetworkClass::connected() const
{
    return A9G_NetworkIsActivated();
}

bool A9GNetworkClass::getIP(char* ip, size_t size) const
{
    return A9G_NetworkGetIp(ip, size);
}

String A9GNetworkClass::ip() const
{
    char buffer[32];
    if (!getIP(buffer, sizeof(buffer)))
        return String("");
    return String(buffer);
}

int A9GNetworkClass::signalQuality() const
{
    return A9G_NetworkGetSignalQuality();
}

Network_Status_t A9GNetworkClass::status() const
{
    return A9G_NetworkGetStatus();
}
