#ifndef ACOUSTIC_HPP_
#define ACOUSTIC_HPP_

#include "resource/Sound.hpp"

namespace reprize
{
class EnvDepInfo;

namespace aud
{
class Speaker;

class Acoustic
{
public:
    Acoustic(EnvDepInfo* depinfo_) {}
    virtual ~Acoustic(void) {}

    virtual const bool init(void) { return true; }

    virtual void begin_play(void) {}
    virtual void finish_play(void) {}

    virtual void release(void) {}
    virtual void test(void) {}
    void send_speaker(const res::Sound* snd_)
        {
            RE_DBG("snd: %s", snd_->get_name().c_str());
        }

private:
    Speaker* spk[100];
};
}
}

#endif
