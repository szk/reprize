#ifndef NSEV_KEYBOARD_HPP_
#define NSEV_KEYBOARD_HPP_

#include "Keyboard.hpp"
#include "NSDepInfo.hpp"

namespace reprize
{
namespace inp
{
class NSEvKeyboard : public Keyboard
{
public:
    NSEvKeyboard(NSDepInfo* dep_info_) : Keyboard(dep_info_), ns_info(dep_info_)
        {}
    virtual ~NSEvKeyboard(void) {}

    virtual const bool init(void) { Keyboard::init(); return true; }
    virtual void restore(void) {}
    virtual void reset(void) { Keyboard::reset(); }
    virtual void check(InputCode pressed_[], InputCode released_[]) {}

protected:
    NSDepInfo* ns_info;
};
}
}
#endif
