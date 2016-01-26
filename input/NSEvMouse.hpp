#ifndef NSEV_MOUSE_HPP_
#define NSEV_MOUSE_HPP_

#include "Mouse.hpp"
#include "NSDepInfo.hpp"

namespace reprize
{
namespace inp
{
class NSEvMouse : public Mouse
{
public:
    NSEvMouse(NSDepInfo* dep_info_) : Mouse(dep_info_), ns_info(dep_info_)
        {
        }
    virtual ~NSEvMouse(void) {}

    virtual const bool init(void)
        {
            return false;
        }
    virtual void restore(void)
        {
        }
    virtual void check(InputCode pressed_[], InputCode released_[])
        {
            x = ns_info->get_x_pos();
            y = ns_info->get_y_pos();
            if (ns_info->get_clicked())
            {
                add_key(K_ESCAPE, pressed_);
                ns_info->set_click(false);
            }
            if (ns_info->get_scale() > 0.5)
            { add_key(K_S, pressed_); }
            else if (ns_info->get_scale() < 1.0)
            { add_key(K_S, released_); }
        }
    virtual void reset(void)
        {
        }

protected:
    NSDepInfo* ns_info;
};
}
}
#endif
