#ifndef A9G_LOCATION_BRIDGE_H
#define A9G_LOCATION_BRIDGE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool A9G_LocationValid(void);
bool A9G_LocationHasFix(void);
int A9G_LocationFixType(void);
int A9G_LocationFixQuality(void);
int A9G_LocationSatellites(void);
float A9G_LocationLatitude(void);
float A9G_LocationLongitude(void);
float A9G_LocationAltitude(void);
float A9G_LocationSpeedKph(void);
float A9G_LocationCourse(void);

#ifdef __cplusplus
}
#endif

#endif
