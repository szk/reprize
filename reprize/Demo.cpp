#include "reprize.hpp"

using namespace reprize;

#ifdef WIN32
#include <windows.h>
#include <tchar.h>

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine, int       nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
    g_finish = g_focus = false;
    Core core;

    // Others
    vid::Video* video = NULL;
    inp::Input* input = NULL;
    net::Network* network = NULL;
    aud::Audio* audio = NULL;
    tmr::Timer* timer = NULL;
    res::Resource* resource = NULL;

#ifdef WIN32
    Win32Device* device = new Win32Device(__argc, __argv);
    video = device->create_video();
    input = device->create_input();
#else
    X11Device* x11_device = new X11Device(argc, argv);
    video = x11_device->create_video();
    input = x11_device->create_input();
    UNIXDevice* device = new UNIXDevice(argc, argv);
#endif

    network = device->create_network();
    audio = device->create_audio();
    timer = device->create_timer();
    resource = device->create_resource();

    core.cl_init(video, audio, input, network, resource, timer);
    core.cl_load(DIR_BASE);

    core.srv_init(network, resource, timer);
    core.srv_load(DIR_BASE);
    core.srv_start(NET_DEFAULT_PORT);

    core.cl_start();

    while (!g_finish) { core.cl_loop(); }

    return 0;
}
