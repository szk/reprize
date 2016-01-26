#include "Common.hpp"

#include "WMsgKeyboard.hpp"
#include "WMsgMouse.hpp"
#include "WMsgOptIO.hpp"

#include "WMsgBase.hpp"

using namespace reprize;
using namespace inp;

WMsgBase::WMsgBase(reprize::Win32DepInfo* dep_info)
    : InputBase(dep_info), win32_info(dep_info)
{
}

WMsgBase::~WMsgBase(void)
{
}

const bool WMsgBase::init(void)
{
    return InputBase::init();
}

void WMsgBase::restore(void)
{
}

void WMsgBase::check(Keyboard* keyboard_, Mouse* mouse_, OptIO* opt_io_)
{
    while (PeekMessage(win32_info->get_msg(), NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(win32_info->get_msg());
        DispatchMessage(win32_info->get_msg());

        switch (win32_info->get_msg()->message)
        {
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
//         case WM_MENUCHAR:
        case WM_CHAR:
//         case WM_SYSCHAR:
//         case WM_SYSDEADCHAR:
            keyboard_->check(pressed, released);
            break;
        case WM_MOUSEMOVE:
        case WM_LBUTTONUP:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDOWN:
            mouse_->check(pressed, released);
            break;
        case WM_CLOSE:
            g_finish = true;
            break;
        default: break;
        }
    }
}
