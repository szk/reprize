#include "Common.hpp"

#include "video/GLWin32Screen.hpp"

#include "resource/XML.hpp"
#include "resource/Font.hpp"
#include "resource/Mesh.hpp"
#include "resource/Geometry.hpp"
#include "resource/Model.hpp"

#include "resource/PlaneGlyph.hpp"
#include "resource/loader/GLTexLoader.hpp"
#include "resource/GeomGlyph.hpp"

#include "GLWin32FontLoader.hpp"

using namespace reprize;
using namespace res;
using namespace vid;
using namespace std;

GLWin32FontLoader::GLWin32FontLoader(Win32DepInfo* depinfo_)
    : FontLoader(depinfo_)
{
}

GLWin32FontLoader::~GLWin32FontLoader(void)
{
}

void GLWin32FontLoader::init(void)
{
}

const bool GLWin32FontLoader::load(const Str& filename_, const Str& path_)
{
/*
	if (filename_.compare(filename_.size() - 4, filename_.size() - 1, ".ttf"))
    { return false; }

    FT_Face* ft_face = new FT_Face;
    const FT_Long DEFAULT_FACE_INDEX = 0;
    FT_Error err = FT_New_Face(ttf->get_lib(), path_.c_str(),
                               DEFAULT_FACE_INDEX, ft_face);

    if (err) { DEL(ft_face); return false; }
//     int numGlyphs = (*ft_face)->num_glyphs;
//     bool hasKerningTable = FT_HAS_KERNING((*ft_face));

    FT_Set_Char_Size(*ft_face, 0L, 12 * 64, 72, 72);
    err = FT_Load_Glyph(*ft_face, 68, FT_LOAD_NO_HINTING);
    if (err) { return false; }

    FT_GlyphSlot glyph = (*ft_face)->glyph;
    err = FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL);

    if(err || glyph->format != ft_glyph_format_bitmap)
    { RE_DBG("err: %d", err); return false; }

    FT_Bitmap bitmap = glyph->bitmap;

    int destWidth  = bitmap.width;
    int destHeight = bitmap.rows;

    int xOffset = 0, yOffset = 0;

    loaded_node = NULL;

    if(destWidth && destHeight)
    {
        GLuint name;
        glGenTextures(1, &name);
        glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
        glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glBindTexture(GL_TEXTURE_2D, name);
        glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset,
                        destWidth, destHeight, GL_ALPHA, GL_UNSIGNED_BYTE,
                        bitmap.buffer);
        glPopClientAttrib();
        loaded_node = new GLSurface("ttf_test", name);
        RE_DBG(name);
    }

    /// refer 'VarLoader::gen_font'

//     uv[0].X(static_cast<float>(xOffset) / static_cast<float>(width));
//     uv[0].Y(static_cast<float>(yOffset) / static_cast<float>(height));
//     uv[1].X(static_cast<float>( xOffset + destWidth) / static_cast<float>(width));
//     uv[1].Y(static_cast<float>( yOffset + destHeight) / static_cast<float>(height));

//     pos.X(glyph->bitmap_left);
//     pos.Y(glyph->bitmap_top);

//     FTFont* pf
//     pf->Depth(20);
//     pf->CharMap(ft_encoding_unicode);
//    FTGlyph* glyph = pf->MakeGlyph('a');

*/
    return true;
}

Glyph* GLWin32FontLoader::make_tex(const Str& filename_, Int32 x_, Int32 y_,
                                   Int32 width_, Int32 height_)
{
	return NULL;
//    return new PlaneGlyph(tex_, x_, y_, width_, height_);
}

Glyph* GLWin32FontLoader::make_geom(const Str& filename_, uInt32 code_)
{
	return NULL;
//    return new PlaneGlyph(surf_, 0, 0, 0, 0);
}

/*
void GLFontLoader::print_glyph(Int32 code, Float32 x_ratio, Float32 y_ratio)
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0f, 1.0f, 1.0f, 0.0f, -1.0, 1.0);
    glMatrixMode( GL_MODELVIEW );

    glLoadIdentity();
    gluLookAt(	0.0, 0.0, 1.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);

    glTranslatef(x_ratio, y_ratio, 0);
}

*/
