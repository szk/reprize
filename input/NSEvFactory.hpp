#ifndef NSEV_FACTORY_HPP_
#define NSEV_FACTORY_HPP_

#include "InputFactory.hpp"
#include "input/NSEvBase.hpp"
#include "input/NSEvKeyboard.hpp"
#include "input/NSEvMouse.hpp"
#include "input/NSEvOptIO.hpp"

namespace reprize
{
namespace inp
{
// Abstract Factory
class NSEvFactory : public InputFactory
{
public:
    NSEvFactory() {}
    virtual ~NSEvFactory(void) {}

    virtual Input* makeInput(void) const { return new Input; }

    virtual InputBase* makeBase(NSDepInfo* dep_info_) const
        { return new NSEvBase(dep_info_); }

    virtual Keyboard* makeKeyboard(NSDepInfo* dep_info_) const
        { return new NSEvKeyboard(dep_info_); }
    virtual Mouse* makeMouse(NSDepInfo* dep_info_) const
        { return new NSEvMouse(dep_info_); }
    virtual OptIO* makeOptIO(NSDepInfo* dep_info_) const
        { return new NSEvOptIO(dep_info_); }
};
}
}
#endif
