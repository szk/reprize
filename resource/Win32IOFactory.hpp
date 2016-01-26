#ifndef WIN32IO_FACTORY_HPP_
#define WIN32IO_FACTORY_HPP_

#include "ResourceFactory.hpp"

#include "reader/Win32FSReader.hpp"
#include "reader/NetReader.hpp"

namespace reprize
{
namespace res
{
class Win32IOFactory : public ResourceFactory
{
public:
    Win32IOFactory(void) {}
    virtual ~Win32IOFactory(void) {}

    FSReader* makeFSReader(void) const { return new Win32FSReader; }
//    NetReader* makeNetReader(void) const { return NULL }
};
}
}
#endif
