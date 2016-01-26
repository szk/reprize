#ifndef MOUSE_HPP_
#define MOUSE_HPP_

#include "InputBase.hpp"

class EnvDepInfo;

namespace reprize
{
namespace inp
{
class Mouse
{
public:
    Mouse(EnvDepInfo* dep_info) : x(0), y(0), relative(false) {}
    virtual ~Mouse(void) {}

    virtual const bool init(void) { return false; }
    virtual void restore(void) {}
    virtual void check(InputBuffer& pressed_, InputBuffer& released_) {}
    virtual void reset(void) {}
    virtual const bool set_pos(const uInt32 scr_x_, const uInt32 scr_y_)
        { return false; }

    inline void set_relative(const bool relative_) { relative = relative_; }
    inline Int32 get_x(void) { return x; }
    inline Int32 get_y(void) { return y; }

    inline void debug(void)
        {
            if (relative && (x !=0 || y != 0))
            { RE_DBG("%d, %d", x, y); }
        }

protected:
    inline void add_key(InputCode keycode_, InputBuffer& buf_)
        { buf_.Add(keycode_); }

    Int32 x, y;
    bool relative;
};
}
}
#endif
