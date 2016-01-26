#ifndef UNIX_FACTORY_HPP_
#define UNIX_FACTORY_HPP_

#include "ResourceFactory.hpp"

#include "reader/POSIXFSReader.hpp"
#include "reader/NetReader.hpp"

namespace reprize
{
namespace res
{
class UNIXFactory : public ResourceFactory
{
public:
    UNIXFactory(void) {}
    virtual ~UNIXFactory(void) {}

    FSReader* makeFSReader(void) const { return new POSIXFSReader; }
//    NetReader* makeNetReader(void) const { return NULL }
};
}
}
#endif
