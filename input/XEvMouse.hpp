#ifndef XEV_MOUSE_HPP_
#define XEV_MOUSE_HPP_

#include "Mouse.hpp"

namespace reprize
{
class X11DepInfo;

namespace inp
{
class XEvMouse : public Mouse
{
public:
    XEvMouse(X11DepInfo* dep_info_);
    virtual ~XEvMouse(void);

    virtual const bool init(void);
    virtual void restore(void);
    virtual void check(InputBuffer& pressed_, InputBuffer& released_);
    virtual void reset(void);
    virtual const bool set_pos(const uInt32 scr_x_, const uInt32 scr_y_);

protected:
    X11DepInfo* x11_info;
};
}
}
#endif
