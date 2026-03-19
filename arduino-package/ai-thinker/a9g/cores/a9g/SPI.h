#ifndef A9G_ARDUINO_CORE_SPI_H
#define A9G_ARDUINO_CORE_SPI_H

#include <stddef.h>
#include <stdint.h>

#define LSBFIRST 0
#define MSBFIRST 1

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

class SPISettings {
public:
    SPISettings() : clockFreq(1000000), bitOrder(MSBFIRST), dataMode(SPI_MODE0) {}
    SPISettings(uint32_t clockHz, uint8_t order, uint8_t mode)
        : clockFreq(clockHz), bitOrder(order), dataMode(mode) {}

    uint32_t clockFreq;
    uint8_t bitOrder;
    uint8_t dataMode;
};

class SPIClass {
public:
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
};

extern SPIClass SPI;

#endif
