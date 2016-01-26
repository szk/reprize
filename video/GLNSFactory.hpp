#ifndef GLNS_FACTORY_HPP_
#define GLNS_FACTORY_HPP_

#include "VidFactory.hpp"

#include "GLNSScreen.hpp"
#include "GLRenderer.hpp"
#include "resource/loader/GLTexLoader.hpp"
#include "resource/loader/GLNSFontLoader.hpp"

namespace reprize
{
class NSDepInfo;

namespace vid
{
class GLNSFactory : public VidFactory
{
public:
    GLNSFactory(void) {}
    virtual ~GLNSFactory(void) {}

    virtual Screen* make_screen(NSDepInfo* depinfo_) const
        { return new GLNSScreen(depinfo_); }
    virtual Renderer* make_renderer(void) const
        { return new GLRenderer; }
    virtual res::TexLoader* make_tex_loader(void) const
        { return new res::GLTexLoader; }
    virtual res::FontLoader* make_font_loader(NSDepInfo* depinfo_) const
        { return new res::GLNSFontLoader(depinfo_); }
};
}
}
#endif
