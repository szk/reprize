#ifndef XEV_FACTORY_HPP_
#define XEV_FACTORY_HPP_

#include "InputFactory.hpp"
#include "XEvOptIO.hpp"
#include "XEvBase.hpp"
#include "XEvKeyboard.hpp"
#include "XEvMouse.hpp"
#include "XEvOptIO.hpp"

namespace reprize
{
class X11DepInfo;

namespace inp
{
class XEvFactory : public InputFactory
{
public:
    XEvFactory(void) {}
    virtual ~XEvFactory(void) {}

    virtual InputBase* makeBase(X11DepInfo* depinfo_) const { return new XEvBase(depinfo_); }

    virtual Keyboard* makeKeyboard(X11DepInfo* depinfo_) const { return new XEvKeyboard(depinfo_); }
    virtual Mouse* makeMouse(X11DepInfo* depinfo_) const { return new XEvMouse(depinfo_); }
    virtual OptIO* makeOptIO(X11DepInfo* depinfo_) const { return new XEvOptIO(depinfo_); }

protected:
    X11DepInfo* x11_info;
};
}
}
#endif
