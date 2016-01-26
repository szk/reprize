#ifndef X11_DEP_INFO_HPP_
#define X11_DEP_INFO_HPP_

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include "EnvDepInfo.hpp"

namespace reprize
{
class X11DepInfo : public EnvDepInfo
{
public:
    X11DepInfo(int ac, char* av[])
	: dpy(NULL), scr(0), win(0), argc(ac), argv(av)
	{}
    virtual ~X11DepInfo(void) {}

    void set_dpy(Display* display) { dpy = display; }
    Display* get_dpy(void) { return dpy; }

    void set_scr(int screen) { scr = screen; }
    int get_scr(void) { return scr; }

    void set_win(Window window) { win = window; }
    Window get_win(void) { return win; }

    XEvent* get_event(void) { return &event; }

    void set_del_atom(Atom atom) { wm_del_win = atom; }
    Atom* get_del_atom(void) { return &wm_del_win; }

    int get_argc(void) { return argc; }
    char** get_argv(void) { return argv; }

protected:
    Display* dpy;
    int scr;
    Window win;

    XEvent event;
    Atom wm_del_win;

    int argc;
    char** argv;
};
}
#endif
