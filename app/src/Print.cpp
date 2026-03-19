#include "Print.h"
#include "Printable.h"
#include "WString.h"

#include <stdio.h>

size_t Print::write(const char* str)
{
    size_t total = 0;

    if (str == NULL)
        return 0;

    while (*str != '\0')
        total += write((uint8_t)*str++);

    return total;
}

size_t Print::write(const uint8_t* buffer, size_t size)
{
    size_t total = 0;
    size_t i = 0;

    if (buffer == NULL)
        return 0;

    for (i = 0; i < size; ++i)
        total += write(buffer[i]);

    return total;
}

size_t Print::print(const char* str)
{
    return write(str);
}

size_t Print::print(const String& str)
{
    return write(str.c_str());
}

size_t Print::print(const Printable& printable)
{
    return printable.printTo(*this);
}

size_t Print::print(char c)
{
    return write((uint8_t)c);
}

size_t Print::print(int value)
{
    return printNumber(value);
}

size_t Print::print(unsigned int value)
{
    return printUnsigned(value);
}

size_t Print::print(long value)
{
    return printNumber(value);
}

size_t Print::print(unsigned long value)
{
    return printUnsigned(value);
}

size_t Print::println()
{
    return write("\r\n");
}

size_t Print::println(const char* str)
{
    return print(str) + println();
}

size_t Print::println(const String& str)
{
    return print(str) + println();
}

size_t Print::println(const Printable& printable)
{
    return print(printable) + println();
}

size_t Print::println(int value)
{
    return print(value) + println();
}

size_t Print::println(unsigned int value)
{
    return print(value) + println();
}

size_t Print::println(long value)
{
    return print(value) + println();
}

size_t Print::println(unsigned long value)
{
    return print(value) + println();
}

size_t Print::printNumber(long value)
{
    char buffer[24];
    snprintf(buffer, sizeof(buffer), "%ld", value);
    return write(buffer);
}

size_t Print::printUnsigned(unsigned long value)
{
    char buffer[24];
    snprintf(buffer, sizeof(buffer), "%lu", value);
    return write(buffer);
}
