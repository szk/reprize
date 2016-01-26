#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "reprize.hpp"

#include "reprize/bp/Boid.hpp"
#include "reprize/bp/LSystem.hpp"
#include "reprize/bp/Primitive.hpp"
#include "reprize/bp/Terrain.hpp"

using namespace reprize;

Core core;
NSDevice* ns_device;

void engine_init(int width, int height)
{
    // Stores the size to this OpenGL application.

    // Creates all the OpenGL objects necessary to an application.
//    createFrameAndRenderbuffers_pre();

    g_finish = g_focus = false;

    // Others
    vid::Video* video = NULL;
    inp::Input* input = NULL;
    net::Network* network = NULL;
    aud::Audio* audio = NULL;
    tmr::Timer* timer = NULL;
    res::Resource* resource = NULL;

    ns_device = new NSDevice(0, NULL);
    video = ns_device->create_video();
    input = ns_device->create_input();

    UNIXDevice* device = new UNIXDevice(0, NULL);
    network = device->create_network();
    audio = device->create_audio();
    timer = device->create_timer();
    resource = device->create_resource();

    core.cl_init(video, audio, input, network, resource, timer);

    core.add_blueprint(new Boid);
    core.add_blueprint(new LSystem);
    core.add_blueprint(new Primitive);
    core.add_blueprint(new Terrain);

    core.cl_load(DIR_BASE);

    core.srv_init(network, resource, timer);
    core.srv_load(DIR_BASE);
    core.srv_start(NET_DEFAULT_PORT);

    core.cl_start();

    g_log->printf("reprize initializing finished");
}

void engine_loop(void)
{
    core.cl_loop();
}

void set_pos(Float32 x_, Float32 y_)
{
    ns_device->set_pos(x_, y_);
}

void set_click(const bool clicked_)
{
    ns_device->set_click(clicked_);
}

void set_scale(const Float32 scale_)
{
    ns_device->set_scale(scale_);
}
