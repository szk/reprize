#ifndef WIN32_DEP_INFO_HPP_
#define WIN32_DEP_INFO_HPP_

#define NOMINMAX
#include <windows.h>
#include <tchar.h>
#include <basetsd.h>

#ifdef DIRECTX
#include <ddraw.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#endif

#include "GL/gl.h"
#include "EnvDepInfo.hpp"

class Win32DepInfo : public reprize::EnvDepInfo
{
public:
    Win32DepInfo(int ac, char* av[])
        : EnvDepInfo() {}
    virtual ~Win32DepInfo(void) {}

    HINSTANCE get_hinst(void) { return h_inst; }
    void set_hinst(HINSTANCE h_inst_) { h_inst = h_inst_; }

    HWND get_hwnd(void) { return h_wnd; }
    void set_hwnd(HWND h_wnd_) { h_wnd = h_wnd_; }

    MSG* get_msg(void) { return &msg; }

#ifdef DIRECTX
    LPDIRECTINPUT8* get_dinp(void) { return dinp; }
    void set_dinp(LPDIRECTINPUT8 *dnp) { dinp = dnp; }
    LPDIRECTINPUTDEVICE8* get_dinp_key(void) { return dinp_key; }
    void set_dinp_key(LPDIRECTINPUTDEVICE8 *dnp_key) { dinp_key = dnp_key; }
    LPDIRECTINPUTDEVICE8* get_dinpMouse(void) { return dinpMouse; }
    void set_dinpMouse(LPDIRECTINPUTDEVICE8 *dnpMouse) { dinpMouse = dnpMouse; }
    LPDIRECTINPUTDEVICE8* get_dinp_stick(void) { return dinp_stick; }
    void set_dinp_stick(LPDIRECTINPUTDEVICE8 *dnp_stick)
        { dinp_stick = dnp_stick; }
#endif

private:
    // handler
    HINSTANCE h_inst;
    HWND h_wnd;
    // message
    MSG msg;

#ifdef DIRECTX
    // directX
    LPDIRECTINPUT8* dinp;
    LPDIRECTINPUTDEVICE8* dinp_key;
    LPDIRECTINPUTDEVICE8* dinpMouse;
    LPDIRECTINPUTDEVICE8* dinp_stick;
#endif
};

#endif
