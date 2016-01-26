#ifndef GLWIN32_FONT_LOADER_HPP_
#define GLWIN32_FONT_LOADER_HPP_

#include "FontLoader.hpp"

namespace reprize
{
namespace res
{
class Surface;
class Font;

class GLWin32FontLoader : public FontLoader
{
public:
	GLWin32FontLoader(Win32DepInfo* depinfo_);
    virtual ~GLWin32FontLoader(void);

    virtual void init(void);
    virtual const bool load(const Str& filename_, const Str& path_);

private:
    virtual Glyph* make_tex(const Str& filename_, Int32 x_, Int32 y_,
                            Int32 width_, Int32 height_);
    virtual Glyph* make_geom(const Str& filename_, uInt32 code_);
};
}
}
#endif
