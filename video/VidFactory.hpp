#ifndef VID_FACTORY_HPP_
#define VID_FACTORY_HPP_

#include "Screen.hpp"

#include "Renderer.hpp"
#include "resource/loader/TexLoader.hpp"
#include "resource/loader/FontLoader.hpp"

namespace reprize
{
namespace vid
{
// Abstract factory
class VidFactory
{
public:
    VidFactory(void) {}
    virtual ~VidFactory(void) {}

    Video* make_video(void) const { return new Video; }

    virtual Screen* make_screen(EnvDepInfo* depinfo) const
        { return new Screen(depinfo); }
    virtual Renderer* make_renderer(void) const { return new Renderer; }
    virtual res::TexLoader* make_tex_loader(void) const = 0;
    virtual res::FontLoader* make_font_loader(EnvDepInfo* depinfo_) const
        { return new res::FontLoader(depinfo_); }
};
}
}

#endif
