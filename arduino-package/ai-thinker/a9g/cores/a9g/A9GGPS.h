#ifndef A9G_ARDUINO_GPS_H
#define A9G_ARDUINO_GPS_H

#include "Stream.h"

class A9GGPSClass : public Stream {
public:
    bool begin();
    bool end();
    bool isOpen() const;

    virtual size_t write(unsigned char value);
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();
};

extern A9GGPSClass GPS;

#endif
