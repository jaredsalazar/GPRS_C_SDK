#include "A9GLocationBridge.h"

#include "gps_parse.h"
#include "minmea.h"

bool A9G_LocationValid(void)
{
    GPS_Info_t* info = Gps_GetInfo();
    return info != NULL && info->rmc.valid;
}

bool A9G_LocationHasFix(void)
{
    GPS_Info_t* info = Gps_GetInfo();
    int gpsFix = 0;
    int bdsFix = 0;

    if (info == NULL)
        return false;

    gpsFix = info->gsa[0].fix_type;
    bdsFix = info->gsa[1].fix_type;
    return (gpsFix > bdsFix ? gpsFix : bdsFix) >= 2 && info->gga.fix_quality > 0;
}

int A9G_LocationFixType(void)
{
    GPS_Info_t* info = Gps_GetInfo();
    int gpsFix = 0;
    int bdsFix = 0;

    if (info == NULL)
        return 0;

    gpsFix = info->gsa[0].fix_type;
    bdsFix = info->gsa[1].fix_type;
    return gpsFix > bdsFix ? gpsFix : bdsFix;
}

int A9G_LocationFixQuality(void)
{
    GPS_Info_t* info = Gps_GetInfo();
    return info == NULL ? 0 : info->gga.fix_quality;
}

int A9G_LocationSatellites(void)
{
    GPS_Info_t* info = Gps_GetInfo();
    return info == NULL ? 0 : info->gga.satellites_tracked;
}

float A9G_LocationLatitude(void)
{
    GPS_Info_t* info = Gps_GetInfo();
    return info == NULL ? 0.0f : minmea_tocoord(&info->rmc.latitude);
}

float A9G_LocationLongitude(void)
{
    GPS_Info_t* info = Gps_GetInfo();
    return info == NULL ? 0.0f : minmea_tocoord(&info->rmc.longitude);
}

float A9G_LocationAltitude(void)
{
    GPS_Info_t* info = Gps_GetInfo();
    return info == NULL ? 0.0f : minmea_tofloat(&info->gga.altitude);
}

float A9G_LocationSpeedKph(void)
{
    GPS_Info_t* info = Gps_GetInfo();
    return info == NULL ? 0.0f : minmea_tofloat(&info->vtg.speed_kph);
}

float A9G_LocationCourse(void)
{
    GPS_Info_t* info = Gps_GetInfo();
    return info == NULL ? 0.0f : minmea_tofloat(&info->vtg.true_track_degrees);
}
