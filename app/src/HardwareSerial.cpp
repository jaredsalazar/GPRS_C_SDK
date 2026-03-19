#include "HardwareSerial.h"

HardwareSerial Serial(UART1);
HardwareSerial Serial2(UART2);

HardwareSerial::HardwareSerial(UART_Port_t port) : _port(port), _isOpen(false), _peek(-1) {}

bool HardwareSerial::begin(unsigned long baud)
{
    UART_Config_t config;
    config.baudRate = mapBaud(baud);
    config.dataBits = UART_DATA_BITS_8;
    config.stopBits = UART_STOP_BITS_1;
    config.parity = UART_PARITY_NONE;
    config.rxCallback = NULL;
    config.errorCallback = NULL;
    config.useEvent = false;

    _isOpen = UART_Init(_port, config);
    _peek = -1;
    return _isOpen;
}

size_t HardwareSerial::write(uint8_t value)
{
    return write(&value, 1);
}

size_t HardwareSerial::write(const uint8_t* buffer, size_t size)
{
    if (!_isOpen || buffer == NULL || size == 0)
        return 0;
    return UART_Write(_port, (uint8_t*)buffer, (uint32_t)size);
}

size_t HardwareSerial::write(const char* str)
{
    return Print::write(str);
}

uint32_t HardwareSerial::readBytes(uint8_t* buffer, uint32_t length, uint32_t timeoutMs)
{
    uint32_t count = 0;

    if (!_isOpen || buffer == NULL || length == 0)
        return 0;

    if (_peek >= 0)
    {
        buffer[count++] = (uint8_t)_peek;
        _peek = -1;
    }

    if (count < length)
        count += UART_Read(_port, buffer + count, length - count, timeoutMs);

    return count;
}

int HardwareSerial::available()
{
    return _peek >= 0 ? 1 : 0;
}

int HardwareSerial::availableForWrite()
{
    return _isOpen ? 64 : 0;
}

int HardwareSerial::read()
{
    return read(0);
}

int HardwareSerial::read(uint32_t timeoutMs)
{
    uint8_t value = 0;

    if (_peek >= 0)
    {
        value = (uint8_t)_peek;
        _peek = -1;
        return value;
    }

    if (!_isOpen)
        return -1;

    if (UART_Read(_port, &value, 1, timeoutMs) == 1)
        return value;

    return -1;
}

int HardwareSerial::peek()
{
    if (_peek >= 0)
        return _peek;

    _peek = read(0);
    return _peek;
}

void HardwareSerial::flush()
{
}

size_t HardwareSerial::print(const char* str)
{
    return Print::print(str);
}

size_t HardwareSerial::print(char c)
{
    return Print::print(c);
}

size_t HardwareSerial::print(int value)
{
    return Print::print(value);
}

size_t HardwareSerial::print(unsigned int value)
{
    return Print::print(value);
}

size_t HardwareSerial::print(long value)
{
    return Print::print(value);
}

size_t HardwareSerial::print(unsigned long value)
{
    return Print::print(value);
}

size_t HardwareSerial::println()
{
    return Print::println();
}

size_t HardwareSerial::println(const char* str)
{
    return Print::println(str);
}

size_t HardwareSerial::println(int value)
{
    return Print::println(value);
}

size_t HardwareSerial::println(unsigned int value)
{
    return Print::println(value);
}

size_t HardwareSerial::println(long value)
{
    return Print::println(value);
}

size_t HardwareSerial::println(unsigned long value)
{
    return Print::println(value);
}

UART_Baud_Rate_t HardwareSerial::mapBaud(unsigned long baud) const
{
    switch (baud)
    {
        case 1200: return UART_BAUD_RATE_1200;
        case 2400: return UART_BAUD_RATE_2400;
        case 4800: return UART_BAUD_RATE_4800;
        case 9600: return UART_BAUD_RATE_9600;
        case 14400: return UART_BAUD_RATE_14400;
        case 19200: return UART_BAUD_RATE_19200;
        case 28800: return UART_BAUD_RATE_28800;
        case 33600: return UART_BAUD_RATE_33600;
        case 38400: return UART_BAUD_RATE_38400;
        case 57600: return UART_BAUD_RATE_57600;
        case 62000: return UART_BAUD_RATE_62000;
        case 115200: return UART_BAUD_RATE_115200;
        case 230400: return UART_BAUD_RATE_230400;
        case 460800: return UART_BAUD_RATE_460800;
        case 921600: return UART_BAUD_RATE_921600;
        case 1300000: return UART_BAUD_RATE_1300000;
        case 1625000: return UART_BAUD_RATE_1625000;
        case 2166700: return UART_BAUD_RATE_2166700;
        case 3250000: return UART_BAUD_RATE_3250000;
        default: return UART_BAUD_RATE_115200;
    }
}
