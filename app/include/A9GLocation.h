#ifndef A9G_ARDUINO_LOCATION_H
#define A9G_ARDUINO_LOCATION_H

#include <stdint.h>

class A9GLocationClass {
public:
    bool valid() const;
    bool hasFix() const;
    int fixType() const;
    int fixQuality() const;
    int satellites() const;
    float latitude() const;
    float longitude() const;
    float altitude() const;
    float speedKph() const;
    float course() const;

    bool setUpdateInterval(uint16_t intervalMs);
};

extern A9GLocationClass Location;

#endif
