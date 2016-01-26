#include "Common.hpp"

using namespace reprize;
using namespace srv;

Server::Server(void)
  : node(NULL)
{
    pvt = new srv_pvt;
    node = new res::Node("Server");
}

Server::~Server(void)
{
}

