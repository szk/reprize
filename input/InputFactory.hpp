#ifndef INPUT_FACTORY_HPP_
#define INPUT_FACTORY_HPP_

#include "InputBase.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "OptIO.hpp"

namespace reprize
{
namespace inp
{
// Abstract Factory
class InputFactory
{
public:
    InputFactory() {}
    virtual ~InputFactory(void) {}

    virtual Input* makeInput(void) const { return new Input; }

    virtual InputBase* makeBase(EnvDepInfo* dep_info_) const
        { return new InputBase(dep_info_); }

    virtual Keyboard* makeKeyboard(EnvDepInfo* dep_info_) const
        { return new Keyboard(dep_info_); }
    virtual Mouse* makeMouse(EnvDepInfo* dep_info_) const
        { return new Mouse(dep_info_); }
    virtual OptIO* makeOptIO(EnvDepInfo* dep_info_) const
        { return new OptIO(dep_info_); }
};
}
}
#endif
