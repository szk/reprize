#ifndef AL_WIN32_FACTORY_HPP_
#define AL_WIN32_FACTORY_HPP_

#include "AudFactory.hpp"

#include "ALWin32Acoustic.hpp"
#include "resource/loader/ALSndLoader.hpp"

namespace reprize
{
namespace aud
{
class ALWin32Factory : public AudFactory
{
public:
    ALWin32Factory(void) {}
    virtual ~ALWin32Factory(void) {}

    virtual Acoustic* make_acoustic(reprize::EnvDepInfo* depinfo_) const
        { return new ALWin32Acoustic(depinfo_); }
    virtual res::SndLoader* make_snd_loader(reprize::EnvDepInfo* depinfo_) const
        { return new res::ALSndLoader(depinfo_); }
};
}
}
#endif
