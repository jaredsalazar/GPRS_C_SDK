#include "A9GUDP.h"

#include "api_socket.h"

#ifdef read
#undef read
#endif

#ifdef write
#undef write
#endif

#ifdef connect
#undef connect
#endif

A9GUDP::A9GUDP() : _socket(-1), _peek(-1), _connected(false), _localPort(0), _remotePort(0), _remoteIp(), _txLength(0)
{
}

size_t A9GUDP::write(uint8_t value)
{
    if (_txLength >= sizeof(_txBuffer))
        return 0;
    _txBuffer[_txLength++] = value;
    return 1;
}

int A9GUDP::available()
{
    return _peek >= 0 ? 1 : 0;
}

int A9GUDP::readIntoPeek()
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

int A9GUDP::read()
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

int A9GUDP::peek()
{
    if (_peek >= 0)
        return _peek;
    return readIntoPeek();
}

void A9GUDP::flush()
{
}

uint8_t A9GUDP::begin(uint16_t port)
{
    _localPort = port;
    return 1;
}

int A9GUDP::beginPacket(IPAddress ip, uint16_t port)
{
    return beginPacket(ip.toString().c_str(), port);
}

int A9GUDP::beginPacket(const char* host, uint16_t port)
{
    char ip[16];
    int index = 0;

    stop();
    _txLength = 0;
    _peek = -1;
    _remotePort = port;

    if (host == NULL)
        return 0;

    ip[0] = '\0';
    if (DNS_GetHostByName2((const uint8_t*)host, (uint8_t*)ip) != 0)
    {
        while (host[index] != '\0' && index + 1 < (int)sizeof(ip))
        {
            ip[index] = host[index];
            ++index;
        }
        ip[index] = '\0';
    }

    _remoteIp.fromString(ip);
    _socket = Socket_TcpipConnect((TCP_UDP_t)1, ip, port);
    _connected = _socket >= 0;
    return _connected ? 1 : 0;
}

int A9GUDP::endPacket()
{
    int result = 0;

    if (!_connected || _socket < 0)
        return 0;

    result = Socket_TcpipWrite(_socket, _txBuffer, (uint16_t)_txLength);
    _txLength = 0;
    return result >= 0 ? 1 : 0;
}

void A9GUDP::stop()
{
    if (_socket >= 0)
        Socket_TcpipClose(_socket);
    _socket = -1;
    _peek = -1;
    _connected = false;
    _txLength = 0;
}
