#include "Common.hpp"

#include "Socket.hpp"
#include "Connection.hpp"

using namespace reprize;
using namespace net;

Network::Network(void)
  : node(NULL)
{
    node = new res::Node("Network");
}

Network::~Network(void)
{
}

res::Node* Network::get_node(void)
{
    return node;
}

const bool Network::init(void)
{
    return false;
}

void Network::set_view(inf::Interface* inf_)
{
    inf_ = NULL;
}

Connection* Network::add_passive(const Char* hostname,
                                 const Char* port, const Char* protocol)
{
    return NULL;//socket->create_passive(hostname, port, protocol);
}

Connection* Network::add_active(const Char* hostname_,
                                const Char* port_,  const Char* protocol_)
{
    return NULL;//socket->create_active(hostname_, port_, protocol_);
}

const bool Network::close_socket(void) // close the socket and all connections
{
//     bool closed = true;

//     for (conn_itr = conn.begin(); conn_itr != conn.end(); ++conn_itr)
//     {
//         if (!(*conn_itr)->close_conn()) closed = false;
//     }
//     return closed;
    return false;
}

uInt32 Network::get_output(void) // byte
{
    return 0;
}

uInt32 Network::get_input(void)  // byte
{
    return 0;
}

uInt32 Network::http_request(const Str& url_,
                             const Str& user_, const Str& pass_)
{
    return 0;
}
