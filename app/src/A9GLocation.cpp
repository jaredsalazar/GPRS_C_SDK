#include "A9GLocation.h"

#include "A9GLocationBridge.h"
#include "gps.h"

A9GLocationClass Location;

bool A9GLocationClass::valid() const
{
    return A9G_LocationValid();
}

bool A9GLocationClass::hasFix() const
{
    return A9G_LocationHasFix();
}

int A9GLocationClass::fixType() const
{
    return A9G_LocationFixType();
}

int A9GLocationClass::fixQuality() const
{
    return A9G_LocationFixQuality();
}

int A9GLocationClass::satellites() const
{
    return A9G_LocationSatellites();
}

float A9GLocationClass::latitude() const
{
    return A9G_LocationLatitude();
}

float A9GLocationClass::longitude() const
{
    return A9G_LocationLongitude();
}

float A9GLocationClass::altitude() const
{
    return A9G_LocationAltitude();
}

float A9GLocationClass::speedKph() const
{
    return A9G_LocationSpeedKph();
}

float A9GLocationClass::course() const
{
    return A9G_LocationCourse();
}

bool A9GLocationClass::setUpdateInterval(uint16_t intervalMs)
{
    return GPS_SetOutputInterval(intervalMs);
}
