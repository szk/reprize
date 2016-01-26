#include "Common.hpp"

#include "misc/NSDepInfo.hpp"

#include "video/VidFactory.hpp"
#include "video/GLNSFactory.hpp"
#include "video/GLNSScreen.hpp"
#include "video/GLRenderer.hpp"

#include "input/InputFactory.hpp"

#include "input/NSEvFactory.hpp"
//#include "input/NSEvBase.hpp"
//#include "input/NSEvKeyboard.hpp"
//#include "input/NSEvMouse.hpp"
//#include "input/NSEvOptIO.hpp"

// declaration is in reprize.hpp

using namespace reprize;

NSDevice::NSDevice(int ac, char** av)
{
    pvt = new ns_pvt;
    pvt->ns_dep_info = new NSDepInfo(ac, av);
    // pvt->ns_dep_info->set_dpy(XOpenDisplay(0));
    // if (pvt->ns_dep_info->get_dpy() == NULL) { g_log->printf("NSDevice: failed to init"); }
#ifdef UNDEF
    pvt->ns_dep_info->set_scr(DefaultScreen(pvt->ns_dep_info->get_dpy()));
#endif
}

NSDevice::~NSDevice(void)
{
    DEL(pvt);
}

vid::Video* NSDevice::create_video(void)
{
    // XXX
    vid::GLNSFactory factory;

    vid::Video* video = factory.make_video();
    vid::Screen* screen = factory.make_screen(pvt->ns_dep_info);
    vid::Renderer* renderer = factory.make_renderer();

    res::TexLoader* tex_loader = factory.make_tex_loader();
    res::FontLoader* font_loader = factory.make_font_loader(pvt->ns_dep_info);

    video->set_screen(screen);
    video->set_renderer(renderer);
    video->set_tex_loader(tex_loader);
    video->set_font_loader(font_loader);

    return video;
}

inp::Input* NSDevice::create_input(void)
{
    inp::NSEvFactory factory;

    inp::Input* input = factory.makeInput();
    inp::InputBase* base = factory.makeBase(pvt->ns_dep_info);
    inp::Keyboard* keyboard = factory.makeKeyboard(pvt->ns_dep_info);
    inp::Mouse* mouse = factory.makeMouse(pvt->ns_dep_info);
    inp::OptIO* opt_io = factory.makeOptIO(pvt->ns_dep_info);

    input->set_base(base);
    input->set_keyboard(keyboard);
    input->set_mouse(mouse);
    input->set_opt_io(opt_io);

    return input;
}

void NSDevice::set_pos(Float32 x_, Float32 y_)
{
    pvt->ns_dep_info->set_pos(x_, y_);
}

void NSDevice::set_click(const bool clicked_)
{
    pvt->ns_dep_info->set_click(clicked_);
}

void NSDevice::set_scale(const Float32 scale_)
{
    pvt->ns_dep_info->set_scale(scale_);
}
