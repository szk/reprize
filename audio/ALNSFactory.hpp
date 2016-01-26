#ifndef AL_FACTORY_HPP_
#define AL_FACTORY_HPP_

#include "AudFactory.hpp"

#include "ALUNIXAcoustic.hpp"
#include "Speaker.hpp"
#include "resource/loader/ALSndLoader.hpp"

namespace reprize
{
namespace aud
{
class ALUNIXFactory : public AudFactory
{
public:
    ALUNIXFactory(void) {}
    virtual ~ALUNIXFactory(void) {}

    virtual Acoustic* make_acoustic(reprize::EnvDepInfo* depinfo_) const
        { return new ALUNIXAcoustic(depinfo_); }
    virtual Speaker* make_speaker(void) const { return new Speaker; }
    virtual res::SndLoader* make_snd_loader(reprize::EnvDepInfo* depinfo_) const
        { return new res::ALSndLoader(depinfo_); }
};
}
}

#endif
