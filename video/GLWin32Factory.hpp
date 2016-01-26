#ifndef GLWIN32_FACTORY_HPP_
#define GLWIN32_FACTORY_HPP_

#include "VidFactory.hpp"

#include "GLWin32Screen.hpp"
#include "GLRenderer.hpp"
#include "resource/loader/GLTexLoader.hpp"
#include "resource/loader/GLWin32FontLoader.hpp"

namespace reprize
{
namespace vid
{
class GLWin32Factory : public VidFactory
{
public:
    GLWin32Factory(void) {}
    virtual ~GLWin32Factory(void) {}

    virtual Screen* make_screen(reprize::Win32DepInfo* depinfo_)
        const { return new GLWin32Screen(depinfo_); }
    virtual Renderer* make_renderer(void) const { return new GLRenderer; }
    virtual res::TexLoader* make_tex_loader(void) const
        { return new res::GLTexLoader; }
    virtual res::FontLoader* make_font_loader(reprize::Win32DepInfo* depinfo_)
        const { return new res::GLWin32FontLoader(depinfo_); }
};
}
}
#endif
