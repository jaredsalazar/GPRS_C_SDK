#include "A9GClient.h"

#include "Arduino.h"
#include "api_socket.h"
#include "api_os.h"

#ifdef read
#undef read
#endif

#ifdef write
#undef write
#endif

#ifdef connect
#undef connect
#endif

A9GClient::A9GClient() : _socket(-1), _peek(-1), _connected(false)
{
}

size_t A9GClient::write(uint8_t value)
{
    if (!_connected || _socket < 0)
        return 0;

    return Socket_TcpipWrite(_socket, &value, 1) == 1 ? 1 : 0;
}

int A9GClient::available()
{
    return _peek >= 0 ? 1 : 0;
}

int A9GClient::readIntoPeek()
{
    uint8_t value = 0;
    int result = 0;

    if (!_connected || _socket < 0)
        return -1;

    result = Socket_TcpipRead(_socket, &value, 1);
    if (result <= 0)
    {
        if (result < 0)
            _connected = false;
        return -1;
    }

    _peek = value;
    return _peek;
}

int A9GClient::read()
{
    int value = 0;

    if (_peek >= 0)
    {
        value = _peek;
        _peek = -1;
        return value;
    }

    return readIntoPeek() >= 0 ? read() : -1;
}

int A9GClient::peek()
{
    if (_peek >= 0)
        return _peek;

    return readIntoPeek();
}

void A9GClient::flush()
{
}

int A9GClient::connect(IPAddress ip, uint16_t port)
{
    return connect(ip.toString().c_str(), port);
}

int A9GClient::connect(const char* host, uint16_t port)
{
    char ip[16];

    stop();
    _peek = -1;

    if (host == NULL)
        return 0;

    ip[0] = '\0';
    if (DNS_GetHostByName2((const uint8_t*)host, (uint8_t*)ip) != 0)
    {
        const char* source = host;
        int index = 0;
        while (source[index] != '\0' && index + 1 < (int)sizeof(ip))
        {
            ip[index] = source[index];
            ++index;
        }
        ip[index] = '\0';
    }

    _socket = Socket_TcpipConnect(TCP, ip, port);
    _connected = _socket >= 0;
    return _connected ? 1 : 0;
}

uint8_t A9GClient::connected()
{
    return _connected ? 1 : 0;
}

uint32_t A9GClient::readBytes(uint8_t* buffer, uint32_t length, uint32_t timeoutMs)
{
    uint32_t count = 0;
    uint32_t start = millis();

    if (buffer == NULL || length == 0)
        return 0;

    while (count < length)
    {
        int value = read();
        if (value >= 0)
        {
            buffer[count++] = (uint8_t)value;
            continue;
        }

        if (timeoutMs == 0 || millis() - start >= timeoutMs)
            break;

        OS_Sleep(1);
    }

    return count;
}

void A9GClient::stop()
{
    if (_socket >= 0)
        Socket_TcpipClose(_socket);
    _socket = -1;
    _peek = -1;
    _connected = false;
}
