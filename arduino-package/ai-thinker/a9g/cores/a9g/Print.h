#ifndef A9G_ARDUINO_CORE_PRINT_H
#define A9G_ARDUINO_CORE_PRINT_H

#include <stddef.h>
#include <stdint.h>

class String;
class Printable;

class Print {
public:
    virtual ~Print() {}
    virtual size_t write(uint8_t value) = 0;

    size_t write(const char* str);
    size_t write(const uint8_t* buffer, size_t size);
    size_t print(const char* str);
    size_t print(const String& str);
    size_t print(const Printable& printable);
    size_t print(char c);
    size_t print(int value);
    size_t print(unsigned int value);
    size_t print(long value);
    size_t print(unsigned long value);
    size_t println();
    size_t println(const char* str);
    size_t println(const String& str);
    size_t println(const Printable& printable);
    size_t println(int value);
    size_t println(unsigned int value);
    size_t println(long value);
    size_t println(unsigned long value);
};

#endif
