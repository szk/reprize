#include "Common.hpp"

#ifdef OPENAL
#include "audio/ALUNIXFactory.hpp"
#include "audio/ALUNIXAcoustic.hpp"
#include "audio/Speaker.hpp"
#else
#include "audio/AudFactory.hpp"
#endif

#include "resource/reader/POSIXFSReader.hpp"
#include "resource/reader/zlibReader.hpp"
#include "resource/reader/NetReader.hpp"

#include "network/NetFactory.hpp"
#include "resource/UNIXFactory.hpp"
#include "misc/UNIXTimer.hpp"

// declaration is in reprize.hpp

using namespace reprize;

UNIXDevice::UNIXDevice(int argc_, char* argv_[])
{
    pvt = new unix_pvt;
}

UNIXDevice::~UNIXDevice(void)
{
}

tmr::Timer* UNIXDevice::create_timer(void)
{
    tmr::Timer* timer = tmr::UNIXTimer::Instance();
    return timer;
}

aud::Audio* UNIXDevice::create_audio(void)
{
#ifdef OPENAL
    aud::ALUNIXFactory factory;
#else
    aud::AudFactory factory;
#endif

    aud::Audio* audio = factory.make_audio();
    aud::Acoustic* acoustic = factory.make_acoustic(NULL);
    res::SndLoader* snd_loader = factory.make_snd_loader(NULL);

    audio->set_acoustic(acoustic);
    audio->set_snd_loader(snd_loader);
    return audio;
}

net::Network* UNIXDevice::create_network(void)
{
    net::NetFactory net_factory;
    net::Network* network = net_factory.makeNetwork();
//    connection = net_factory.makeConnection();

    return network;
}

res::Resource* UNIXDevice::create_resource(void)
{
    // XXX
    res::UNIXFactory factory;

    res::Resource* resource = factory.makeResource();
    res::FSReader* fs_reader = factory.makeFSReader();
    res::zlibReader* zlib_reader = factory.makezlibReader();
    res::NetReader* net_reader = factory.makeNetReader();

    resource->add_reader(fs_reader);
    resource->add_reader(zlib_reader);
    resource->add_reader(net_reader);

    return resource;
}
