#ifndef A9G_ARDUINO_CORE_WIRE_H
#define A9G_ARDUINO_CORE_WIRE_H

#include <stddef.h>
#include <stdint.h>

#include "Stream.h"

class TwoWire : public Stream {
public:
    bool begin();
    void end();
    void setClock(uint32_t frequency);
    void beginTransmission(uint8_t address);
    virtual size_t write(uint8_t value);
    size_t write(const uint8_t* data, size_t quantity);
    uint8_t endTransmission(bool sendStop = true);
    uint8_t requestFrom(uint8_t address, uint8_t quantity, bool sendStop = true);
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();
};

extern TwoWire Wire;

#endif
