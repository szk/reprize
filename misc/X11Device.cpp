#include "Common.hpp"

#include "misc/X11DepInfo.hpp"

#include "video/VidFactory.hpp"
#include "video/GLX11Factory.hpp"
#include "video/GLX11Screen.hpp"
#include "video/GLRenderer.hpp"
#include "resource/loader/GLTexLoader.hpp"
#include "resource/loader/GLX11FontLoader.hpp"

#include "input/InputFactory.hpp"
#include "input/XEvFactory.hpp"
#include "input/XEvBase.hpp"
#include "input/XEvKeyboard.hpp"
#include "input/XEvMouse.hpp"
#include "input/XEvOptIO.hpp"

// declaration is in reprize.hpp

using namespace reprize;

X11Device::X11Device(int ac, char** av)
{
    pvt = new x11_pvt;
    pvt->x11_dep_info = new X11DepInfo(ac, av);
    pvt->x11_dep_info->set_dpy(XOpenDisplay(0));
    if (pvt->x11_dep_info->get_dpy() == NULL)
    { g_log->printf("X11Device: failed to init"); }
    pvt->x11_dep_info->set_scr(DefaultScreen(pvt->x11_dep_info->get_dpy()));
}

X11Device::~X11Device(void)
{
    DEL(pvt);
}

vid::Video* X11Device::create_video(void)
{
    // XXX
    vid::GLX11Factory factory;

    vid::Video* video = factory.make_video();
    vid::Screen* screen = factory.make_screen(pvt->x11_dep_info);
    vid::Renderer* renderer = factory.make_renderer();

    res::TexLoader* tex_loader = factory.make_tex_loader();
    res::FontLoader* font_loader = factory.make_font_loader(pvt->x11_dep_info);

    video->set_screen(screen);
    video->set_renderer(renderer);
    video->set_tex_loader(tex_loader);
    video->set_font_loader(font_loader);

    return video;
}

inp::Input* X11Device::create_input(void)
{
    // XXX
    inp::XEvFactory factory;

    inp::Input* input = factory.makeInput();
    inp::InputBase* base = factory.makeBase(pvt->x11_dep_info);
    inp::Keyboard* keyboard = factory.makeKeyboard(pvt->x11_dep_info);
    inp::Mouse* mouse = factory.makeMouse(pvt->x11_dep_info);
    inp::OptIO* opt_io = factory.makeOptIO(pvt->x11_dep_info);

    input->set_base(base);
    input->set_keyboard(keyboard);
    input->set_mouse(mouse);
    input->set_opt_io(opt_io);

    return input;
}
