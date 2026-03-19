#ifndef A9G_ARDUINO_WIRE_H
#define A9G_ARDUINO_WIRE_H

#include <stddef.h>
#include <stdint.h>

#include "api_hal_i2c.h"
#include "Stream.h"

class TwoWire : public Stream {
public:
    explicit TwoWire(I2C_ID_t bus = I2C1);

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

private:
    enum { BUFFER_LENGTH = 32 };

    I2C_ID_t _bus;
    bool _initialized;
    I2C_FREQ_t _frequency;
    uint8_t _txAddress;
    uint8_t _txBuffer[BUFFER_LENGTH];
    size_t _txLength;
    uint8_t _rxBuffer[BUFFER_LENGTH];
    size_t _rxLength;
    size_t _rxIndex;

    bool initBus();
};

extern TwoWire Wire;

#endif
