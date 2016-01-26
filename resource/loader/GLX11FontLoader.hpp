#ifndef GLX11_FONT_LOADER_HPP_
#define GLX11_FONT_LOADER_HPP_

#include "FontLoader.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#include FT_TRUETYPE_TAGS_H
#include FT_TRUETYPE_TABLES_H
#include FT_OUTLINE_H

namespace reprize
{
class X11DepInfo;

namespace res
{
class Texture;
class Font;
class Glyph;

class GLX11FontLoader : public FontLoader
{
public:
    GLX11FontLoader(X11DepInfo* depinfo_);
    virtual ~GLX11FontLoader(void);

    virtual void init(void);
    const bool load(const Str& filename_, const Str& path_);

protected:
    virtual const bool bmptobuf(uChar* src_, uInt32* dst_, uInt32 size_);
    virtual Glyph* make_tex(const Str& filename_, Int32 x_, Int32 y_,
                            Int32 width_, Int32 height_);
    virtual Glyph* make_geom(const Str& filename_, uInt32 code_);

    static GLvoid tess_begin(GLenum type_, Geometry* geom_);
    static GLvoid tess_end(Geometry* geom_);
    static GLvoid tess_vertex(void* data_, Geometry* geom_);
    static GLvoid tess_combine(GLdouble coords_[3], void* d_[4], GLfloat w_[4],
                               void** out_data, Geometry* geom_);
    static void tess_error(GLenum err_, Geometry* geom_);

    static int move_to(FT_Vector* to, Geometry* user);
    static int line_to(FT_Vector* to, Geometry* user);
    static int conic_to(FT_Vector* control, FT_Vector* to, Geometry* user);
    static int cubic_to(FT_Vector* control1, FT_Vector* control2, FT_Vector* to,
                        Geometry* user);

    class TTF* ttf;
};
}
}
#endif
