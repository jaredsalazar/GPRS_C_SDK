#include "SPI.h"

#include "api_os.h"

SPIClass SPI(SPI1);

SPISettings::SPISettings() : clockFreq(1000000), bitOrder(MSBFIRST), dataMode(SPI_MODE0) {}

SPISettings::SPISettings(uint32_t clockHz, uint8_t order, uint8_t mode)
    : clockFreq(clockHz), bitOrder(order), dataMode(mode) {}

SPIClass::SPIClass(SPI_ID_t spi) : _spi(spi), _initialized(false), _settings() {}

bool SPIClass::begin()
{
    return applySettings(_settings);
}

void SPIClass::end()
{
    if (_initialized)
    {
        SPI_Close(_spi);
        _initialized = false;
    }
}

void SPIClass::beginTransaction(const SPISettings& settings)
{
    applySettings(settings);
}

void SPIClass::endTransaction()
{
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
    SPISettings settings = _settings;
    settings.bitOrder = bitOrder;
    applySettings(settings);
}

void SPIClass::setDataMode(uint8_t dataMode)
{
    SPISettings settings = _settings;
    settings.dataMode = dataMode;
    applySettings(settings);
}

void SPIClass::setClockDivider(uint32_t clockHz)
{
    SPISettings settings = _settings;
    settings.clockFreq = clockHz;
    applySettings(settings);
}

uint8_t SPIClass::transfer(uint8_t data)
{
    uint8_t tx = data;
    uint8_t rx = 0;

    if (!_initialized && !begin())
        return 0;

    if (_settings.bitOrder == LSBFIRST)
        tx = reverseBits(tx);

    if (SPI_Write(_spi, &tx, 1) != 1)
        return 0;

    while (!SPI_IsTxDone(_spi))
        OS_Sleep(1);

    if (SPI_Read(_spi, &rx, 1) != 1)
        return 0;

    if (_settings.bitOrder == LSBFIRST)
        rx = reverseBits(rx);

    return rx;
}

uint16_t SPIClass::transfer16(uint16_t data)
{
    uint16_t value = 0;

    if (_settings.bitOrder == LSBFIRST)
    {
        value = (uint16_t)transfer((uint8_t)(data & 0xff));
        value |= (uint16_t)transfer((uint8_t)((data >> 8) & 0xff)) << 8;
    }
    else
    {
        value = (uint16_t)transfer((uint8_t)((data >> 8) & 0xff)) << 8;
        value |= transfer((uint8_t)(data & 0xff));
    }

    return value;
}

void SPIClass::transfer(void* buffer, size_t count)
{
    uint8_t* data = (uint8_t*)buffer;
    size_t i = 0;

    if (data == NULL)
        return;

    for (i = 0; i < count; ++i)
        data[i] = transfer(data[i]);
}

bool SPIClass::applySettings(const SPISettings& settings)
{
    SPI_Config_t config;

    if (_initialized)
        SPI_Close(_spi);

    if (settings.clockFreq == 0)
        config.freq = 1000000;
    else if (settings.clockFreq > SPI_FREQ_MAX)
        config.freq = SPI_FREQ_MAX;
    else
        config.freq = settings.clockFreq;

    config.cs = SPI_CS_0;
    config.txMode = SPI_MODE_DIRECT_POLLING;
    config.rxMode = SPI_MODE_DIRECT_POLLING;
    config.line = SPI_LINE_4;
    config.txOnly = false;
    config.cpol = (settings.dataMode & SPI_MODE2) ? 1 : 0;
    config.cpha = (settings.dataMode & SPI_MODE1) ? 1 : 0;
    config.csActiveLow = true;
    config.dataBits = SPI_DATA_BITS_8;
    config.irqHandler = NULL;
    config.irqMask.rxOverflow = false;
    config.irqMask.txFinish = false;
    config.irqMask.txDmaDone = false;
    config.irqMask.rxFinish = false;
    config.irqMask.rxDmaDone = false;

    _initialized = SPI_Init(_spi, config);
    if (_initialized)
        _settings = settings;

    return _initialized;
}

uint8_t SPIClass::reverseBits(uint8_t value)
{
    uint8_t reversed = 0;
    uint8_t i = 0;

    for (i = 0; i < 8; ++i)
    {
        reversed <<= 1;
        reversed |= (value & 0x01);
        value >>= 1;
    }

    return reversed;
}
