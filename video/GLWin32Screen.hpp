#ifndef GLWIN32_SCREEN_HPP_
#define GLWIN32_SCREEN_HPP_

#include "Screen.hpp"

namespace reprize
{
namespace vid
{
class GLWin32Screen : public Screen
{
public:
    GLWin32Screen(Win32DepInfo* depinfo_);
    virtual ~GLWin32Screen(void);

    virtual void init(void);
    virtual void appear(void);

    virtual void begin_paint(void);
    virtual void finish_paint(void);
    virtual const bool flip(void);

    virtual void release(void);

private:
    HICON create_icon(void);
    Win32DepInfo* win32info;
    HWND hwnd;
    HDC hdc;
    HINSTANCE hinst;
    PAINTSTRUCT ps;
};
}
}
#endif
