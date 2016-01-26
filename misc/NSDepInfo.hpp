#ifndef NS_DEP_INFO_HPP_
#define NS_DEP_INFO_HPP_

#include "EnvDepInfo.hpp"

namespace reprize
{
class NSDepInfo : public EnvDepInfo
{
public:
    NSDepInfo(int ac_, char* av_[])
	: argc(ac_), argv(av_)
	{}
    virtual ~NSDepInfo(void) {}

    // void set_dpy(uInt32* display) { dpy = display; }
    // uInt32* get_dpy(void) { return dpy; }

    // void set_scr(int screen) { scr = screen; }
    // int get_scr(void) { return scr; }

    // void set_win(uInt32 window) { win = window; }
    // uInt32 get_win(void) { return win; }

    // uInt32* get_event(void) { return &event; }

    // void set_del_atom(uInt32 atom) { wm_del_win = atom; }
    // uInt32* get_del_atom(void) { return &wm_del_win; }
    void set_pos(Float32 x_, Float32 y_) { x = x_; y = y_; }
    void set_click(const bool clicked_) { clicked = clicked_; }
    void set_scale(const Float32 scale_) { scale = scale_; }
    const Int32 get_x_pos(void) { return static_cast<const Int32>(x); }
    const Int32 get_y_pos(void) { return static_cast<const Int32>(y); }
    const Float32 get_scale(void) { return scale; }
    const bool get_clicked(void) { return clicked; }

    int get_argc(void) { return argc; }
    char** get_argv(void) { return argv; }

protected:
    // uInt32* dpy;
    // uInt32 scr, win, event, wm_del_win;
    Float32 x, y, scale;
    bool clicked;
    int argc;
    char** argv;
};
}
#endif
