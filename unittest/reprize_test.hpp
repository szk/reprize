#ifndef REPRIZE_TEST_
#define REPRIZE_TEST_

#include "reprize.hpp"

TEST(Reprize, external)
{
    int argc = 1;
    char* argv[] = {"./reprize"};
#ifdef WIN32
    Win32Device device(argc, argv);

    Vision* vision = device.create_vision();
    Input* input = device.create_input();
#else
    UNIXDevice device(argc, argv);
    X11Device aux_device(argc, argv);

    Vision* vision = aux_device.create_vision();
    Input* input = aux_device.create_input();
#endif

    Timer* timer = device.create_timer();
    Sound* sound = device.create_sound();

    Network* network = device.create_network();
    Resource* resource = device.create_resource();

    CHECK(vision != NULL);
    CHECK(input != NULL);
    CHECK(timer != NULL);
    CHECK(sound);
    CHECK(network != NULL);
    CHECK(resource != NULL);

    CHECK(resource->init());

    Interface interface;
    CHECK(interface.init(input, resource->get_vfs()));

    Physics physics;
    CHECK(physics.init(resource->get_vfs()));

    Console console;
    MemHunk memhunk;
}

#endif
