#include "Stream.h"

#include <stddef.h>

static bool isDigitChar(int value)
{
    return value >= '0' && value <= '9';
}

int Stream::read(uint32_t timeoutMs)
{
    (void)timeoutMs;
    return read();
}

uint32_t Stream::readBytes(uint8_t* buffer, uint32_t length, uint32_t timeoutMs)
{
    uint32_t count = 0;

    if (buffer == NULL || length == 0)
        return 0;

    while (count < length)
    {
        int value = read(timeoutMs);
        if (value < 0)
            break;
        buffer[count++] = (uint8_t)value;
    }

    return count;
}

uint32_t Stream::readBytesUntil(char terminator, char* buffer, uint32_t length, uint32_t timeoutMs)
{
    uint32_t count = 0;

    if (buffer == NULL || length == 0)
        return 0;

    while (count < (length - 1))
    {
        int value = read(timeoutMs);
        if (value < 0 || value == terminator)
            break;
        buffer[count++] = (char)value;
    }

    buffer[count] = '\0';
    return count;
}

bool Stream::find(char target)
{
    while (1)
    {
        int value = read();
        if (value < 0)
            return false;
        if (value == target)
            return true;
    }
}

bool Stream::find(const char* target)
{
    size_t matched = 0;

    if (target == NULL || target[0] == '\0')
        return true;

    while (1)
    {
        int value = read();
        if (value < 0)
            return false;

        if (value == target[matched])
        {
            ++matched;
            if (target[matched] == '\0')
                return true;
        }
        else
        {
            matched = (value == target[0]) ? 1 : 0;
        }
    }
}

long Stream::parseInt(uint32_t timeoutMs)
{
    long value = 0;
    bool negative = false;
    bool foundDigit = false;

    while (1)
    {
        int current = peek();
        if (current < 0)
            return 0;

        if (current == '-' && !foundDigit)
        {
            negative = true;
            read(timeoutMs);
            break;
        }

        if (isDigitChar(current))
            break;

        read(timeoutMs);
    }

    while (1)
    {
        int current = peek();
        if (!isDigitChar(current))
            break;

        foundDigit = true;
        current = read(timeoutMs);
        value = (value * 10) + (current - '0');
    }

    if (!foundDigit)
        return 0;

    return negative ? -value : value;
}
