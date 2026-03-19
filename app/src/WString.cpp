#include "WString.h"

#include "api_os.h"

#include <stdio.h>
#include <string.h>

String::String() : _buffer(NULL), _length(0)
{
    assign("", 0);
}

String::String(const char* str) : _buffer(NULL), _length(0)
{
    size_t len = stringLength(str);
    assign(str, len);
}

String::String(char c) : _buffer(NULL), _length(0)
{
    char temp[2];
    temp[0] = c;
    temp[1] = '\0';
    assign(temp, 1);
}

String::String(int value) : _buffer(NULL), _length(0)
{
    char temp[24];
    snprintf(temp, sizeof(temp), "%d", value);
    assign(temp, stringLength(temp));
}

String::String(unsigned int value) : _buffer(NULL), _length(0)
{
    char temp[24];
    snprintf(temp, sizeof(temp), "%u", value);
    assign(temp, stringLength(temp));
}

String::String(long value) : _buffer(NULL), _length(0)
{
    char temp[24];
    snprintf(temp, sizeof(temp), "%ld", value);
    assign(temp, stringLength(temp));
}

String::String(unsigned long value) : _buffer(NULL), _length(0)
{
    char temp[24];
    snprintf(temp, sizeof(temp), "%lu", value);
    assign(temp, stringLength(temp));
}

String::String(float value, unsigned char decimalPlaces) : _buffer(NULL), _length(0)
{
    char temp[32];
    snprintf(temp, sizeof(temp), "%.*f", (int)decimalPlaces, value);
    assign(temp, stringLength(temp));
}

String::String(double value, unsigned char decimalPlaces) : _buffer(NULL), _length(0)
{
    char temp[32];
    snprintf(temp, sizeof(temp), "%.*f", (int)decimalPlaces, value);
    assign(temp, stringLength(temp));
}

String::String(const String& other) : _buffer(NULL), _length(0)
{
    assign(other.c_str(), other.length());
}

String::~String()
{
    if (_buffer != NULL)
        OS_Free(_buffer);
}

String& String::operator=(const String& other)
{
    if (this != &other)
        assign(other.c_str(), other.length());
    return *this;
}

String& String::operator=(const char* str)
{
    assign(str, stringLength(str));
    return *this;
}

String& String::operator+=(const String& other)
{
    append(other.c_str(), other.length());
    return *this;
}

String& String::operator+=(const char* str)
{
    append(str, stringLength(str));
    return *this;
}

String& String::operator+=(char c)
{
    char temp[2];
    temp[0] = c;
    temp[1] = '\0';
    append(temp, 1);
    return *this;
}

bool String::operator==(const String& other) const
{
    return strcmp(c_str(), other.c_str()) == 0;
}

bool String::operator==(const char* str) const
{
    if (str == NULL)
        return _length == 0;
    return strcmp(c_str(), str) == 0;
}

bool String::operator!=(const String& other) const
{
    return !(*this == other);
}

bool String::operator!=(const char* str) const
{
    return !(*this == str);
}

const char* String::c_str() const
{
    return _buffer != NULL ? _buffer : "";
}

size_t String::length() const
{
    return _length;
}

bool String::isEmpty() const
{
    return _length == 0;
}

int String::indexOf(char c) const
{
    size_t i = 0;
    for (i = 0; i < _length; ++i)
    {
        if (_buffer[i] == c)
            return (int)i;
    }
    return -1;
}

int String::indexOf(const char* str) const
{
    size_t needleLen = stringLength(str);
    size_t i = 0;

    if (str == NULL || needleLen == 0 || needleLen > _length)
        return -1;

    for (i = 0; i + needleLen <= _length; ++i)
    {
        if (memcmp(_buffer + i, str, needleLen) == 0)
            return (int)i;
    }

    return -1;
}

bool String::startsWith(const char* prefix) const
{
    size_t prefixLen = stringLength(prefix);
    if (prefix == NULL || prefixLen > _length)
        return false;
    return memcmp(c_str(), prefix, prefixLen) == 0;
}

bool String::endsWith(const char* suffix) const
{
    size_t suffixLen = stringLength(suffix);
    if (suffix == NULL || suffixLen > _length)
        return false;
    return memcmp(c_str() + (_length - suffixLen), suffix, suffixLen) == 0;
}

String String::substring(size_t from) const
{
    return substring(from, _length);
}

String String::substring(size_t from, size_t to) const
{
    String result;

    if (from >= _length || to <= from)
    {
        result = "";
        return result;
    }

    if (to > _length)
        to = _length;

    result.assign(c_str() + from, to - from);
    return result;
}

bool String::assign(const char* str, size_t len)
{
    char* buffer;

    buffer = (char*)OS_Malloc(len + 1);

    if (buffer == NULL)
        return false;

    if (str != NULL && len > 0)
        memcpy(buffer, str, len);
    buffer[len] = '\0';

    if (_buffer != NULL)
        OS_Free(_buffer);

    _buffer = buffer;
    _length = len;
    return true;
}

bool String::append(const char* str, size_t len)
{
    char* buffer;

    if (str == NULL || len == 0)
        return true;

    buffer = (char*)OS_Malloc(_length + len + 1);
    if (buffer == NULL)
        return false;

    if (_buffer != NULL && _length > 0)
        memcpy(buffer, _buffer, _length);
    memcpy(buffer + _length, str, len);
    buffer[_length + len] = '\0';

    if (_buffer != NULL)
        OS_Free(_buffer);

    _buffer = buffer;
    _length += len;
    return true;
}

size_t String::stringLength(const char* str)
{
    size_t len = 0;

    if (str == NULL)
        return 0;

    while (str[len] != '\0')
        ++len;

    return len;
}

String operator+(const String& lhs, const String& rhs)
{
    String result(lhs);
    result += rhs;
    return result;
}

String operator+(const String& lhs, const char* rhs)
{
    String result(lhs);
    result += rhs;
    return result;
}

String operator+(const char* lhs, const String& rhs)
{
    String result(lhs);
    result += rhs;
    return result;
}
