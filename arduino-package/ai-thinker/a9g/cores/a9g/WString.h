#ifndef A9G_ARDUINO_CORE_WSTRING_H
#define A9G_ARDUINO_CORE_WSTRING_H

#include <stddef.h>
#include <stdint.h>

class String {
public:
    String();
    String(const char* str);
    String(char c);
    String(int value);
    String(unsigned int value);
    String(long value);
    String(unsigned long value);
    String(float value, unsigned char decimalPlaces = 2);
    String(double value, unsigned char decimalPlaces = 2);
    String(const String& other);
    ~String();

    String& operator=(const String& other);
    String& operator=(const char* str);
    String& operator+=(const String& other);
    String& operator+=(const char* str);
    String& operator+=(char c);

    bool operator==(const String& other) const;
    bool operator==(const char* str) const;
    bool operator!=(const String& other) const;
    bool operator!=(const char* str) const;

    const char* c_str() const;
    size_t length() const;
    bool isEmpty() const;
    int indexOf(char c) const;
    int indexOf(const char* str) const;
    bool startsWith(const char* prefix) const;
    bool endsWith(const char* suffix) const;
    String substring(size_t from) const;
    String substring(size_t from, size_t to) const;
};

String operator+(const String& lhs, const String& rhs);
String operator+(const String& lhs, const char* rhs);
String operator+(const char* lhs, const String& rhs);

#endif
