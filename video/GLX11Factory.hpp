#ifndef GLX11_FACTORY_HPP_
#define GLX11_FACTORY_HPP_

#include "VidFactory.hpp"

#include "GLX11Screen.hpp"
#include "GLRenderer.hpp"
#include "resource/loader/GLTexLoader.hpp"
#include "resource/loader/GLX11FontLoader.hpp"

namespace reprize
{
class X11DepInfo;

namespace vid
{
class GLX11Factory : public VidFactory
{
public:
    GLX11Factory(void) {}
    virtual ~GLX11Factory(void) {}

    virtual Screen* make_screen(X11DepInfo* depinfo_) const
        { return new GLX11Screen(depinfo_); }
    virtual Renderer* make_renderer(void) const
        { return new GLRenderer; }
    virtual res::TexLoader* make_tex_loader(void) const
        { return new res::GLTexLoader; }
    virtual res::FontLoader* make_font_loader(X11DepInfo* depinfo_) const
        { return new res::GLX11FontLoader(depinfo_); }
};
}
}
#endif
