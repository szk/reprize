#ifndef CONN_UDP_HPP_
#define CONN_UDP_HPP_

#include "Connection.hpp"

namespace reprize
{
namespace net
{
class ConnUDP : public Connection
{
public:
    ConnUDP(const uInt32 max_buff_);
    virtual ~ConnUDP(void);

    virtual const bool close_conn(void);
    virtual const bool await(Int32 conn_fd_);
    virtual void loop(void);

    virtual Char* read(uInt32 count_);
    virtual uInt32 write(Char* str_, uInt32 count_);

protected:
};
}
}
#endif
