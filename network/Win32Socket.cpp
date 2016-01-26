#include "Common.hpp"

#include "Win32Socket.hpp"

using namespace reprize;
using namespace net;

Win32Socket::Win32Socket(void)
    : Socket()
{
}

Win32Socket::~Win32Socket(void)
{
}

const bool Win32Socket::open(const char *hostname, const char *port)
{
    return false;
}
