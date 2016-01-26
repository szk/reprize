#ifndef WMSG_MOUSE_HPP_
#define WMSG_MOUSE_HPP_

#include "Mouse.hpp"

namespace reprize
{
namespace inp
{
class WMsgMouse : public Mouse
{
public:
	WMsgMouse(reprize::Win32DepInfo* dep_info);
    virtual ~WMsgMouse(void);

    virtual const bool init(void);
    virtual void restore(void);
    virtual void check(InputBuffer& pressed_, InputBuffer& released_);
    virtual void reset(void);

private:
    reprize::Win32DepInfo* win32_info;
};
}
}
#endif
