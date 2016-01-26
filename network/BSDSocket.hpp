#ifndef BSD_SOCKET_HPP_
#define BSD_SOCKET_HPP_

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>

#include "Socket.hpp"

namespace reprize
{
namespace net
{
class BSDSocket : public Socket
{
public:
    BSDSocket(void);
    virtual ~BSDSocket(void);

    virtual void init(void);
    virtual Connection* create_passive(const Char* hostname_, const Char* port_, const Char* protocol_);
    virtual Connection* create_active(const Char* hostname_, const Char* port_, const Char* protocol_);

    virtual uInt32 get_pasv_n(void);
    virtual uInt32 get_actv_n(void);

protected:
    virtual Connection* create_passive_TCP(const Char *hostname_, const Char *port_);
    virtual Connection* create_active_TCP(const Char *hostname_, const Char *port_);

    virtual Connection* create_passive_UDP(const Char *hostname_, const Char *port_);
    virtual Connection* create_active_UDP(const Char *hostname_, const Char *port_);

    static void* wait_TCP(void* arg_);
    static void* listen_TCP(void* arg_);

    std::vector<Connection*> conn_v;

    Int32 listenfd;
    uInt32 pasv_n, actv_n;
    Int32 iptr;
    pthread_t tid;
};
}
}
#endif
