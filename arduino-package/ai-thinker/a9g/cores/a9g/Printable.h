#ifndef A9G_ARDUINO_PRINTABLE_H
#define A9G_ARDUINO_PRINTABLE_H

#include <stddef.h>

class Print;

class Printable {
public:
    virtual ~Printable() {}
    virtual size_t printTo(Print& printer) const = 0;
};

#endif
