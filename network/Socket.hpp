#ifndef SOCKET_HPP_
#define SOCKET_HPP_

namespace reprize
{
namespace net
{
class Connection;

class Socket
{
public:
    Socket(void) {}
    virtual ~Socket(void) {}

    virtual void init(void) {}

    Connection* create_dummy(const Char* hostname, const Char* port) { return NULL; }
    virtual Connection* create_passive(const Char* hostname, const Char* port, const Char* protocol) { return NULL; }
    virtual Connection* create_active(const Char* hostname, const Char* port, const Char* protocol) { return NULL; }

    virtual uInt32 get_pasv_n(void) { return 0; }
    virtual uInt32 get_actv_n(void) { return 0; }

protected:
};
}
}
#endif
