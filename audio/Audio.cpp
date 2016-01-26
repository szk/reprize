#include "Common.hpp"

#include "Acoustic.hpp"
#include "Speaker.hpp"
#include "resource/loader/SndLoader.hpp"

using namespace reprize;
using namespace aud;

Audio::Audio(void)
  : node(NULL)
{
    pvt = new aud_pvt;
    node = new res::Node("Audio");
}

Audio::~Audio(void)
{
}

res::Node* Audio::get_node(void)
{
    return node;
}

void Audio::set_view(inf::Interface* inf_)
{
    inf_ = NULL;
}

void Audio::set_acoustic(class Acoustic* acoustic_)
{
    pvt->acoustic = acoustic_;
}

void Audio::set_snd_loader(res::SndLoader* snd_loader_)
{
    pvt->snd_loader = snd_loader_;
}

res::Loader* Audio::get_snd_loader(void)
{
    return pvt->snd_loader;
}

const bool Audio::init(void)
{
    RE_DBG("Initializing");

    if (!pvt->acoustic->init()) { return false; }
    RE_DBG("acoustic initialized");

    RE_DBG("Initialized");

//     pvt->acoustic->test();

    return true;
}

const bool Audio::queue(const res::Sound* snd_)
{
    if (snd_ == NULL) { return false; }
    //
    pvt->acoustic->send_speaker(snd_);
//     if (pvt->acoustic != NULL)
//     {
//         RE_DBG("acoustic is null");
//     }
//     pvt->acoustic->test();
    return false;
}

void Audio::finish(void)
{
}

void Audio::set_frame_d(Ratio d_)
{
}

const bool Audio::play(void)
{
    pvt->acoustic->begin_play();
    pvt->acoustic->finish_play();
    return false;
}

const bool Audio::is_played(void)
{
    return false;
}
