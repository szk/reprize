#include "Common.hpp"

#include "video/GLWin32Factory.hpp"
#include "video/GLWin32Screen.hpp"
#include "video/GLRenderer.hpp"
#include "resource/loader/GLTexLoader.hpp"
#include "resource/loader/GLWin32FontLoader.hpp"

#include "input/InputFactory.hpp"
#include "input/WMsgFactory.hpp"
#include "input/WMsgBase.hpp"
#include "input/WMsgKeyboard.hpp"
#include "input/WMsgMouse.hpp"
#include "input/WMsgOptIO.hpp"

#ifdef OPENAL
#include "audio/ALWin32Factory.hpp"
#include "audio/ALWin32Acoustic.hpp"
#else
#include "audio/AudFactory.hpp"
#endif

#include "resource/reader/Win32FSReader.hpp"
#include "resource/reader/zlibReader.hpp"
#include "resource/reader/NetReader.hpp"

#include "network/NetFactory.hpp"
#include "resource/Win32IOFactory.hpp"
#include "misc/Win32Timer.hpp"

#include "reprize_pvt.hpp"

using namespace reprize;
using namespace vid;
using namespace aud;
using namespace res;

Win32Device::Win32Device(int ac, char** av)
{
    pvt = new win32_pvt;
    pvt->win32_dep_info = new reprize::Win32DepInfo(ac, av);
//     pvt->win32_dep_info->set_scr(DefaultScreen(pvt->win32_dep_info->get_dpy()));
}

Win32Device::~Win32Device(void)
{
    DEL(pvt);
}

vid::Video* Win32Device::create_video(void)
{
    // XXX
    vid::GLWin32Factory factory;

    vid::Video* video = factory.make_video();
    vid::Screen* screen = factory.make_screen(pvt->win32_dep_info);
    vid::Renderer* renderer = factory.make_renderer();
    //
    res::TexLoader* tex_loader = factory.make_tex_loader();
    res::FontLoader* font_loader = factory.make_font_loader(pvt->win32_dep_info);

    video->set_screen(screen);
    video->set_renderer(renderer);
    video->set_tex_loader(tex_loader);
    video->set_font_loader(font_loader);

    return video;
}

inp::Input* Win32Device::create_input(void)
{
    // XXX
    inp::WMsgFactory factory;

    inp::Input* input = factory.makeInput();
    inp::InputBase* base = factory.makeBase(pvt->win32_dep_info);
    inp::Keyboard* keyboard = factory.makeKeyboard(pvt->win32_dep_info);
    inp::Mouse* mouse = factory.makeMouse(pvt->win32_dep_info);
    inp::OptIO* opt_io = factory.makeOptIO(pvt->win32_dep_info);

    input->set_base(base);
    input->set_keyboard(keyboard);
    input->set_mouse(mouse);
    input->set_opt_io(opt_io);

    return input;
}

tmr::Timer* Win32Device::create_timer(void)
{
    tmr::Timer* timer = tmr::Win32Timer::Instance();
    return timer;
}

aud::Audio* Win32Device::create_audio(void)
{
#ifdef OPENAL
    aud::ALWin32Factory factory;
#else
    aud::AudFactory factory;
#endif
    aud::Audio* audio = factory.make_audio();
    aud::Acoustic* acoustic = factory.make_acoustic(pvt->win32_dep_info);
    res::SndLoader* snd_loader = factory.make_snd_loader(pvt->win32_dep_info);

    audio->set_acoustic(acoustic);
    audio->set_snd_loader(snd_loader);

    return audio;
}

net::Network* Win32Device::create_network(void)
{
    net::NetFactory net_factory;
    net::Network* network = net_factory.makeNetwork();
    return network;
}

res::Resource* Win32Device::create_resource(void)
{
    // XXX
    res::Win32IOFactory factory;

    res::Resource* resource = factory.makeResource();
    res::FSReader* fs_reader = factory.makeFSReader();
    res::zlibReader* zlib_reader = factory.makezlibReader();
    res::NetReader* net_reader = factory.makeNetReader();

    resource->add_reader(fs_reader);
    resource->add_reader(zlib_reader);
    resource->add_reader(net_reader);

    return resource;
}
