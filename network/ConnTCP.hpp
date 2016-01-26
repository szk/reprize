#ifndef CONN_TCP_HPP_
#define CONN_TCP_HPP_

#include <sys/types.h>

#ifdef WIN32
#include <winsock.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

#include "Connection.hpp"

#define TCPBUFSIZ 102400

namespace reprize
{
static Char buffer[TCPBUFSIZ];
static Char overall_buffer[TCPBUFSIZ];
static bool available;

namespace net
{
class ConnTCP : public Connection
{
public:
//    ConnTCP(const Int32 fd_, socklen_t len_, const uInt32 max_buff_); // UNIX
    ConnTCP(const Int32 fd_, Int32 len_, const uInt32 max_buff_); // WIN32
    virtual ~ConnTCP(void);

    virtual const bool close_conn(void);
    virtual void loop(void);

    virtual const bool has_connection(void);
    virtual const bool data_available(void);

    virtual Char* read(uInt32 count_);
    virtual Char* read_line(uInt32 count_);

    virtual uInt32 write(Char* str_, uInt32 count_);

    // TCP specific methods
    Int32 get_fd(void);
    sockaddr* get_sockaddr(void);
    Int32 get_len(void); // WIN32
//    socklen_t get_len(void); // UNIX

    void set_sockaddr(sockaddr* addr_);

	void set_len(Int32 len); // WIN32
//	void set_len(socklen_t len_); // UNIX

protected:
    static void str_echo(Int32 sockfd_);
    Int32 fd;
    sockaddr* socket_address;
    Int32 len; // WIN32
//    socklen_t len; // UNIX

    uInt32 buffersize;
};
}
}
#endif
