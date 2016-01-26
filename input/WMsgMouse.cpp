#include "Common.hpp"

#include "WMsgMouse.hpp"

using namespace reprize;
using namespace inp;
using namespace std;

WMsgMouse::WMsgMouse(Win32DepInfo* dep_info)
    : Mouse(dep_info), win32_info(dep_info)
{
}

WMsgMouse::~WMsgMouse(void)
{
}

const bool WMsgMouse::init(void)
{
    ShowCursor(FALSE);
    return true;
}

void WMsgMouse::restore(void)
{
}

void WMsgMouse::check(InputBuffer& pressed_, InputBuffer& released_)
{
    InputCode button = 0;

    switch (win32_info->get_msg()->message)
    {
    case WM_LBUTTONDOWN:
        add_key(M_BUTTON0, pressed_);
        break;
    case WM_LBUTTONUP:
        add_key(M_BUTTON0, released_);
        break;
    case WM_RBUTTONDOWN:
        add_key(M_BUTTON1, pressed_);
        break;
    case WM_RBUTTONUP:
        add_key(M_BUTTON1, released_);
        break;
    case WM_MOUSEMOVE:
    {
        POINT scr_ptr = win32_info->get_msg()->pt;
        ScreenToClient(win32_info->get_hwnd(), &scr_ptr);

        if (relative)
        { x = scr_ptr.x - SCREEN_WIDTH / 2; y = scr_ptr.y - SCREEN_HEIGHT / 2; }
        else { x = scr_ptr.x; y = scr_ptr.y; }
        break;
    }
    default: break;
    }
}

void WMsgMouse::reset(void)
{
    POINT scr_ptr = win32_info->get_msg()->pt;
    ScreenToClient(win32_info->get_hwnd(), &scr_ptr);

    POINT center = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    ClientToScreen(win32_info->get_hwnd(), &center);

    if (relative &&
        !(scr_ptr.x == SCREEN_WIDTH / 2 && scr_ptr.y == SCREEN_HEIGHT / 2))
    { SetCursorPos(center.x, center.y); }
}
