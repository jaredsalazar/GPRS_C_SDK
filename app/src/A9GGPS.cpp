#include "A9GGPS.h"

#include "A9GRuntime.h"

A9GGPSClass GPS;

bool A9GGPSClass::begin()
{
    return A9G_GpsBegin();
}

bool A9GGPSClass::end()
{
    return A9G_GpsEnd();
}

bool A9GGPSClass::isOpen() const
{
    return A9G_GpsIsOpen();
}

size_t A9GGPSClass::write(uint8_t value)
{
    (void)value;
    return 0;
}

int A9GGPSClass::available()
{
    return A9G_GpsAvailable();
}

int A9GGPSClass::read()
{
    return A9G_GpsRead();
}

int A9GGPSClass::peek()
{
    return A9G_GpsPeek();
}

void A9GGPSClass::flush()
{
    A9G_GpsFlush();
}
