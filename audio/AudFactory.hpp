#ifndef AUD_FACTORY_HPP_
#define AUD_FACTORY_HPP_

#include "Acoustic.hpp"
#include "Speaker.hpp"
#include "resource/loader/SndLoader.hpp"

namespace reprize
{
namespace aud
{
// Abstract factory
class AudFactory
{
public:
    AudFactory(void) {}
    virtual ~AudFactory(void) {}

    Audio* make_audio(void) const { return new Audio; }

    virtual Acoustic* make_acoustic(EnvDepInfo* depinfo) const
        { return new Acoustic(depinfo); }
    virtual res::SndLoader* make_snd_loader(EnvDepInfo* depinfo_) const
        { return new res::SndLoader(depinfo_); }
};
}
}

#endif
