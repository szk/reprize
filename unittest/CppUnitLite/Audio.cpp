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
    if (pvt->snd_loader != NULL)
    {
        std::cerr << "snd_loader is not null" << std::endl;
        pvt->snd_loader->load(Str("hoge"), Str("bage"));
    }
    return pvt->snd_loader;
}

const bool Audio::init(void)
{
    g_log->printf("Audio initialize");

    if (!pvt->acoustic->init()) { return false; }
    g_log->printf("acoustic initialized");

    g_log->printf("Initialized: audio");

    pvt->acoustic->test();

    return true;
}

const bool Audio::queue(const res::Sound* snd_)
{
    if (snd_ == NULL)
    { std::cerr << "but its null " << std::endl; return false; }
    std::cerr << "snd is not null" << std::endl;
    std::cerr << "queue :" << snd_->get_name() << std::endl;
    // 
//     pvt->acoustic->send_speaker(snd_);
//     if (pvt->acoustic != NULL)
//     {
//         std::cerr << "acoustic is null" << std::endl;
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
