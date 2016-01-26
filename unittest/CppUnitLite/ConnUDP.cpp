#include "Common.hpp"

#include "ConnUDP.hpp"

using namespace reprize;
using namespace net;

ConnUDP::ConnUDP(uInt32 max_buff_)
    : Connection(max_buff_)
{
}

ConnUDP::~ConnUDP(void)
{
//    delete cliaddr;
}

const bool ConnUDP::await(Int32 conn_fd_)
{
    return false;
}

const bool ConnUDP::close_conn(void)
{
    return false;
}

void ConnUDP::loop(void)
{
/*    pthread_detach(pthread_self());
    str_echo((int) arg);
    close((int) arg);
*/
}

Char* ConnUDP::read(uInt32 count_)
{
    return NULL;
}

uInt32 ConnUDP::write(Char* str_, uInt32 count_)
{
    return 0;
}
