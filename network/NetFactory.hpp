#ifndef NET_FACTORY_HPP_
#define NET_FACTORY_HPP_

#include "Connection.hpp"

namespace reprize
{
namespace net
{
// Abstract Factory
class NetFactory
{
public:
    NetFactory() {}
    virtual ~NetFactory(void) {}

    virtual Network* makeNetwork(void) const { return new Network; }

    virtual Connection* makeConnection(void) const { return new Connection(0); }
};
}
}
#endif
