#include "Wire.h"

TwoWire Wire(I2C1);

TwoWire::TwoWire(I2C_ID_t bus)
    : _bus(bus), _initialized(false), _frequency(I2C_FREQ_100K), _txAddress(0),
      _txLength(0), _rxLength(0), _rxIndex(0)
{
}

bool TwoWire::begin()
{
    return initBus();
}

void TwoWire::end()
{
    if (_initialized)
    {
        I2C_Close(_bus);
        _initialized = false;
    }
}

void TwoWire::setClock(uint32_t frequency)
{
    if (frequency > 100000UL)
        _frequency = I2C_FREQ_400K;
    else
        _frequency = I2C_FREQ_100K;

    if (_initialized)
    {
        I2C_Close(_bus);
        _initialized = false;
    }

    initBus();
}

void TwoWire::beginTransmission(uint8_t address)
{
    _txAddress = address;
    _txLength = 0;
}

size_t TwoWire::write(uint8_t value)
{
    if (_txLength >= BUFFER_LENGTH)
        return 0;

    _txBuffer[_txLength++] = value;
    return 1;
}

size_t TwoWire::write(const uint8_t* data, size_t quantity)
{
    size_t written = 0;

    if (data == NULL)
        return 0;

    while (written < quantity && _txLength < BUFFER_LENGTH)
    {
        _txBuffer[_txLength++] = data[written++];
    }

    return written;
}

uint8_t TwoWire::endTransmission(bool sendStop)
{
    I2C_Error_t result;

    (void)sendStop;

    if (!initBus())
        return 4;

    result = I2C_Transmit(_bus, _txAddress, _txBuffer, (uint16_t)_txLength, I2C_DEFAULT_TIME_OUT);
    _txLength = 0;

    if (result == I2C_ERROR_NONE)
        return 0;

    return 4;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, bool sendStop)
{
    I2C_Error_t result;

    (void)sendStop;

    if (quantity > BUFFER_LENGTH)
        quantity = BUFFER_LENGTH;

    _rxLength = 0;
    _rxIndex = 0;

    if (!initBus())
        return 0;

    result = I2C_Receive(_bus, address, _rxBuffer, quantity, I2C_DEFAULT_TIME_OUT);
    if (result != I2C_ERROR_NONE)
        return 0;

    _rxLength = quantity;
    return quantity;
}

int TwoWire::available()
{
    return (int)(_rxLength - _rxIndex);
}

int TwoWire::read()
{
    if (_rxIndex >= _rxLength)
        return -1;

    return _rxBuffer[_rxIndex++];
}

int TwoWire::peek()
{
    if (_rxIndex >= _rxLength)
        return -1;

    return _rxBuffer[_rxIndex];
}

void TwoWire::flush()
{
    _txLength = 0;
    _rxLength = 0;
    _rxIndex = 0;
}

bool TwoWire::initBus()
{
    I2C_Config_t config;

    if (_initialized)
        return true;

    config.freq = _frequency;
    _initialized = I2C_Init(_bus, config);
    return _initialized;
}
