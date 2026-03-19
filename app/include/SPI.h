#ifndef A9G_ARDUINO_SPI_H
#define A9G_ARDUINO_SPI_H

#include <stddef.h>
#include <stdint.h>

#include "api_hal_spi.h"

#define LSBFIRST 0
#define MSBFIRST 1

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

class SPISettings {
public:
    SPISettings();
    SPISettings(uint32_t clockHz, uint8_t bitOrder, uint8_t dataMode);

    uint32_t clockFreq;
    uint8_t bitOrder;
    uint8_t dataMode;
};

class SPIClass {
public:
    explicit SPIClass(SPI_ID_t spi = SPI1);

    bool begin();
    void end();

    void beginTransaction(const SPISettings& settings);
    void endTransaction();

    void setBitOrder(uint8_t bitOrder);
    void setDataMode(uint8_t dataMode);
    void setClockDivider(uint32_t clockHz);

    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    void transfer(void* buffer, size_t count);

private:
    SPI_ID_t _spi;
    bool _initialized;
    SPISettings _settings;

    bool applySettings(const SPISettings& settings);
    static uint8_t reverseBits(uint8_t value);
};

extern SPIClass SPI;

#endif
