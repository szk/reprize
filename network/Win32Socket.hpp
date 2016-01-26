#ifndef WIN32_SOCKET_HPP_
#define WIN32_SOCKET_HPP_

#include "Socket.hpp"

namespace reprize
{
namespace net
{
class Win32Socket : public Socket
{
public:
    Win32Socket(void);
    virtual ~Win32Socket(void);

    virtual const bool open(const char *hostname, const char *port);

protected:
};
}
}
#endif
