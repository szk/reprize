#ifndef XEV_JOYSTICK_HPP_
#define XEV_JOYSTICK_HPP_

#include "OptIO.hpp"

namespace reprize
{
class X11DepInfo;

namespace inp
{
class XEvOptIO : public OptIO
{
public:
    XEvOptIO(X11DepInfo* dep_info_);
    virtual ~XEvOptIO(void);

    virtual const bool init(void);
    virtual void restore(void);

    virtual Int32 get_stick(void);
    virtual Int32 get_trigger(void);

    virtual void check(void);

protected:
    X11DepInfo* x11_info;
};
}
}
#endif
