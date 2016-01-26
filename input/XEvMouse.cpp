#include "Common.hpp"

#include "misc/X11DepInfo.hpp"

#include "XEvMouse.hpp"

using namespace reprize;
using namespace inp;
using namespace std;

XEvMouse::XEvMouse(X11DepInfo* dep_info_)
    : Mouse(dep_info_), x11_info(dep_info_)
{
}

XEvMouse::~XEvMouse(void)
{
}

const bool XEvMouse::init(void)
{
    return true;
}

void XEvMouse::restore(void)
{
}

void XEvMouse::check(InputBuffer& pressed_, InputBuffer& released_)
{
    InputCode button = 0;

    switch (x11_info->get_event()->xbutton.button)
    {
    case 0: break;
    case 1: button = M_BUTTON0; break;
    case 2: button = M_BUTTON1; break;
    case 3: button = M_BUTTON2; break;
    case 4: button = M_BUTTON3; break;
    case 5: button = M_BUTTON4; break;
    case 6: button = M_BUTTON5; break;
    case 7: button = M_BUTTON6; break;
    default: break;
    }

    switch (x11_info->get_event()->type)
    {
    case ButtonPress:
        add_key(button, pressed_);
        break;
    case ButtonRelease:
        add_key(button, released_);
        break;
    case MotionNotify:
        x = x11_info->get_event()->xmotion.x;
        y = x11_info->get_event()->xmotion.y;
        if (relative) { x -= SCREEN_WIDTH / 2; y -= SCREEN_HEIGHT / 2; }
        break;
    }
}

void XEvMouse::reset(void)
{
    if (relative && !(x == 0 && y == 0))
    {
        XWarpPointer(x11_info->get_dpy(), None, x11_info->get_win(),
                     0, 0, 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    }
    XSync(x11_info->get_dpy(), false);
}

const bool XEvMouse::set_pos(const uInt32 scr_x_, const uInt32 scr_y_)
{
    if (scr_x_ >= SCREEN_WIDTH && scr_y_ >= SCREEN_HEIGHT) { return false; }

    XWarpPointer(x11_info->get_dpy(), None, x11_info->get_win(),
                 0, 0, 0, 0, scr_x_, scr_y_);
    return true;
}
