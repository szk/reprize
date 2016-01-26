#ifndef RESOURCE_FACTORY_HPP_
#define RESOURCE_FACTORY_HPP_

#include "reader/FSReader.hpp"
#include "reader/zlibReader.hpp"
#include "reader/NetReader.hpp"

namespace reprize
{
namespace res
{
// Abstract Factory
class ResourceFactory
{
public:
    ResourceFactory(void) {}
    virtual ~ResourceFactory(void) {}

    virtual Resource* makeResource(void) const { return new Resource; }

    virtual FSReader* makeFSReader(void) const { return new FSReader; }
    virtual zlibReader* makezlibReader(void) const { return new zlibReader; }
    virtual NetReader* makeNetReader(void) const { return new NetReader; }
};
}
}
#endif
