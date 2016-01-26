#include "Common.hpp"

#include "misc/NSDepInfo.hpp"
#include "video/GLNSScreen.hpp"

#include "resource/XML.hpp"
#include "resource/Font.hpp"
#include "resource/Mesh.hpp"
#include "resource/Geometry.hpp"
#include "resource/Model.hpp"

#include "resource/PlaneGlyph.hpp"
#include "resource/loader/GLTexLoader.hpp"
#include "resource/GeomGlyph.hpp"

#include "GLNSFontLoader.hpp"
// #include "TTF.hpp"
// #include "util/libtess/tess.h"

using namespace reprize;
using namespace res;
using namespace vid;
using namespace std;

// XXX 'Font' conflicts typedef in X.h

GLNSFontLoader::GLNSFontLoader(NSDepInfo* depinfo_)
    : FontLoader(depinfo_)//, ttf(NULL)
{
#ifdef UNDEF
    ttf = TTF::Instance();
    if (ttf->init()) { cerr << "FT initialized" << endl; }
#endif
}

GLNSFontLoader::~GLNSFontLoader(void)
{
}

void GLNSFontLoader::init(void)
{
}

const bool GLNSFontLoader::load(const Str& filename_, const Str& path_)
{
#ifdef UNDEF
    if (!compare_extension(filename_, ".ttf")) { return false; }

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
    err = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
//     FT_RENDER_MODE_MONO, FT_RENDER_MODE_NORMAL
    if(err || glyph->format != ft_glyph_format_bitmap)
    { cerr << "err: " << err << endl; return false; }

    FT_Bitmap bitmap = glyph->bitmap;

    int dest_width  = 8;//bitmap.width;
    int dest_height = 8;//bitmap.rows;

    loaded_node = NULL;

    if(dest_width && dest_height && vis_loader)
    {
        uInt32* buf = new uInt32[dest_width * dest_height];
        if (bmptobuf(bitmap.buffer, buf, 8 * 8))
        { loaded_node = vis_loader->build("ttf_test", buf, 3); }
        delete[] buf;
    }
#endif
    return loaded_node != NULL;
}

#ifdef UNDEF

const bool GLNSFontLoader::bmptobuf(uChar* src_, uInt32* dst_, uInt32 size_)
{
    for (Size32 i = 0; i < size_; ++i) { dst_[i] = 0xFFFF00FF; }
    return true;
}

Glyph* GLNSFontLoader::make_vis(const Str& filename_, Int32 x_, Int32 y_,
                                  Int32 width_, Int32 height_)
{
    FT_Face ft_face;
    FTC_FaceID face_id = (FTC_FaceID)'a';

    if (FTC_Manager_LookupFace(ttf->get_mngr(), face_id, &ft_face))
    { return NULL; }

    FTC_ScalerRec scaler;
    scaler.face_id = face_id;
    scaler.width = 0;
    scaler.height = 12 << 6;
    scaler.pixel = 0;
    scaler.x_res = 72;
    scaler.y_res = 72;
    FT_Size fn_size;
    if (FTC_Manager_LookupSize(ttf->get_mngr(), &scaler, &fn_size))
    { return NULL; }

    FTC_ImageTypeRec fn_imgt;
    fn_imgt.face_id = face_id;
    fn_imgt.width = 0;
    fn_imgt.height = 12;
    fn_imgt.flags = FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL;

    Geometry* geom = NULL;
    Int32 x;

    FT_Outline_Funcs olfuncs;
    olfuncs.move_to = (FT_Outline_MoveTo_Func)move_to;
    olfuncs.line_to = (FT_Outline_LineTo_Func)line_to;
    olfuncs.conic_to = (FT_Outline_ConicTo_Func)conic_to;
    olfuncs.cubic_to = (FT_Outline_CubicTo_Func)cubic_to;
    olfuncs.shift = 0;
    olfuncs.delta = 0;

    FT_Int cmap_idx;
    bool useKerning = true;
    FT_UInt previous;
    for (Size32 code = 0; code < 1; ++code)
    {
        cmap_idx = FT_Get_Charmap_Index(ft_face->charmap);
        FT_UInt glyph_idx = FTC_CMapCache_Lookup(ttf->get_ccache(), face_id,
                                                   cmap_idx, 'R');
        if(useKerning && previous != 0 && glyph_idx)
        {
            FT_Vector delta;
            FT_Get_Kerning(ft_face, previous, glyph_idx, ft_kerning_default,
                           &delta);
            x += delta.x >> 6;
        }
        previous = glyph_idx;

        FT_Glyph glyph;
        if (FTC_ImageCache_Lookup(ttf->get_icache(), &fn_imgt, glyph_idx,
                                  &glyph, NULL))
        { return NULL; }

        FT_BitmapGlyph bmp_glyph;
        if (glyph->format == FT_GLYPH_FORMAT_BITMAP)
        {
            bmp_glyph = (FT_BitmapGlyph)glyph;
        }
        else
        {
            FT_Glyph tmp_glyph;
            if (FT_Glyph_Copy(glyph, &tmp_glyph)) { return NULL; }
            if (FT_Glyph_To_Bitmap(&tmp_glyph, FT_RENDER_MODE_NORMAL, 0, 1))
            { return NULL; }
            bmp_glyph = (FT_BitmapGlyph)tmp_glyph;
        }

        FT_Bitmap bmp = bmp_glyph->bitmap;
        Int32 left = bmp_glyph->left;
        Int32 top = bmp_glyph->top;
        uInt32 width = bmp.width;
	uInt32 height = bmp.rows;

        for(Size32 j = 0; j < height; ++j)
        {
            unsigned char* p = bmp.pitch < 0 ?
                &bmp.buffer[(-bmp.pitch * bmp.rows) - bmp.pitch * j] :
                &bmp.buffer[bmp.pitch * j];
            for(Size32 i = 0; i < width; ++i)
            {
            }
	}

        FT_GlyphSlot glyph_slt = ft_face->glyph;
        geom = new Geometry("char");
        FT_Outline_Decompose(&(glyph_slt->outline), &olfuncs, geom);
    }

    return NULL; //new PlaneGlyph(mtl_, x_, y_, width_, height_);
}

Glyph* GLNSFontLoader::make_geom(const Str& filename_, uInt32 code_)
{
    FT_Face ft_face;
    FTC_FaceID face_id = (FTC_FaceID)code_;

    if (FTC_Manager_LookupFace(ttf->get_mngr(), face_id, &ft_face))
    { return NULL; }

    FTC_ScalerRec scaler;
    scaler.face_id = face_id;
    scaler.width = 0;
    scaler.height = 12 << 6;
    scaler.pixel = 0;
    scaler.x_res = 72;
    scaler.y_res = 72;
    FT_Size fn_size;
    if (FTC_Manager_LookupSize(ttf->get_mngr(), &scaler, &fn_size))
    { return NULL; }

    FTC_ImageTypeRec fn_imgt;
    fn_imgt.face_id = face_id;
    fn_imgt.width = 0;
    fn_imgt.height = 12;
    fn_imgt.flags = FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL;
    Geometry* geom = NULL;
    Int32 x;

    FT_Outline_Funcs olfuncs;
    olfuncs.move_to = (FT_Outline_MoveTo_Func)move_to;
    olfuncs.line_to = (FT_Outline_LineTo_Func)line_to;
    olfuncs.conic_to = (FT_Outline_ConicTo_Func)conic_to;
    olfuncs.cubic_to = (FT_Outline_CubicTo_Func)cubic_to;
    olfuncs.shift = 0;
    olfuncs.delta = 0;

    FT_Int cmap_idx;
    bool useKerning = true;
    FT_UInt previous;
    for (Size32 i = 0; i < 1; ++i)
    {
        cmap_idx = FT_Get_Charmap_Index(ft_face->charmap);
        FT_UInt glyph_index = FTC_CMapCache_Lookup(ttf->get_ccache(), face_id,
                                                   cmap_idx, 'R');
        if(useKerning && previous != 0 && glyph_index)
        {
            FT_Vector delta;
            FT_Get_Kerning(ft_face, previous, glyph_index, ft_kerning_default,
                           &delta);
            x += delta.x >> 6;
        }
        previous = glyph_index;

        FT_Glyph glyph;
        if (FTC_ImageCache_Lookup(ttf->get_icache(), &fn_imgt, glyph_index,
                                  &glyph, NULL))
        { return NULL; }

        FT_GlyphSlot glyph_slt = ft_face->glyph;
        geom = new Geometry("char");
        FT_Outline_Decompose(&(glyph_slt->outline), &olfuncs, geom);


        GLUtesselator* t = gluNewTess();
        gluTessCallback(t, GLU_TESS_BEGIN_DATA, (GLvoid (*)())tess_begin);
        gluTessCallback(t, GLU_TESS_VERTEX_DATA, (GLvoid (*)())tess_vertex);
        gluTessCallback(t, GLU_TESS_COMBINE_DATA, (GLvoid (*)())tess_combine);
        gluTessCallback(t, GLU_TESS_END_DATA, (GLvoid (*)())tess_end);
        gluTessCallback(t, GLU_TESS_ERROR_DATA, (GLvoid (*)())tess_error);

        // ft_outline_reverse_fill
        if(glyph_slt->outline.flags & ft_outline_even_odd_fill)
        { gluTessProperty(t, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD); }
        else
        { gluTessProperty(t, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO); }

        GLdouble znormal;
        gluTessProperty(t, GLU_TESS_TOLERANCE, 0);
        gluTessNormal(t, 0.0f, 0.0f, znormal);
        gluTessBeginPolygon(t, geom);

        for(Size32 m_idx = 0; m_idx < geom->get_mesh_size(); ++m_idx)
        {
            const Mesh* mesh = geom->get_mesh(m_idx);
            gluTessBeginContour(t);

            for(Size32 v_idx = 0; v_idx < mesh->get_idx_size(); ++v_idx)
            {
                Vec3 v3 = geom->get_vertex(mesh->get_idx(v_idx));
                const Unit* v = v3.get_array_p();
                gluTessVertex(t, (GLdouble*)v, geom);
            }
            gluTessEndContour(t);
        }
        gluTessEndPolygon(t);
        gluDeleteTess(t);

        uv[0].X(static_cast<Unit>(x_offset) / static_cast<Unit>(width));
        uv[0].Y(static_cast<Unit>(y_offset) / static_cast<Unit>(height));
        uv[1].X(static_cast<Unit>(x_offset+d_width)/static_cast<Unit>(width));
        uv[1].Y(static_cast<Unit>(y_offset+d_height)/static_cast<Unit>(height));
    }

    return new GeomGlyph(geom, 0, 0);
}

void GLNSFontLoader::tess_begin(GLenum type_, Geometry* geom_)
{
    cerr << "b";
//     mesh->Begin( type);
}

void GLNSFontLoader::tess_end(Geometry* geom_)
{
    cerr << "E";
//      mesh->End();
}

void GLNSFontLoader::tess_vertex(void* data_, Geometry* geom_)
{
    cerr << "v";
    GLdouble* vertex = static_cast<GLdouble*>(data_);
    geom_->add_mesh(LINE_LOOP);
    geom_->add_mesh_idx(geom_->get_vertex_n());
    geom_->add_vertex(vertex[0], vertex[1], vertex[2]);

//     mesh->AddPoint(vertex[0], vertex[1], vertex[2]);
}

void GLNSFontLoader::tess_combine(GLdouble coords_[3], void* d_[4],
                                   GLfloat w_[4], void** out_data_,
                                   Geometry* geom_)
{
    cerr << "c";
    geom_->add_mesh(LINE_LOOP);
    geom_->add_mesh_idx(geom_->get_vertex_n());
    geom_->add_vertex((Unit)coords_[0], (Unit)coords_[1], (Unit)coords_[2]);

//     cerr << static_cast<Float64*>(coords_)[0] << ","
//          << static_cast<Float64*>(coords_)[1] << ","
//          << static_cast<Float64*>(coords_)[2] << endl;

//     cerr << coords_[0] << ", " << coords_[1] << ", " << coords_[2] << endl;
    *out_data_ = const_cast<Unit*>
        (geom_->get_vertex_array(geom_->get_vertex_n() - 1));
    geom_->get_vertex(geom_->get_vertex_n() - 1).debug();

//     cerr << ((Float64*)out_data_)[0] << ", " << ((Float64*)out_data_)[1] << ", "
//     << ((Float64*)out_data_)[2] << endl;

//     const FTGL_DOUBLE* vertex = static_cast<const FTGL_DOUBLE*>(coords);
//    *outData = const_cast<FTGL_DOUBLE*>(mesh->Combine(vertex[0],
//                                                       vertex[1],
//                                                       vertex[2]));
}

void GLNSFontLoader::tess_error(GLenum err_, Geometry* geom_)
{
    cerr << "e";
//     mesh->Error( errCode);
}

int GLNSFontLoader::move_to(FT_Vector* to_, Geometry* geom_)
{
    geom_->add_mesh(LINE_LOOP);
    geom_->add_mesh_idx(geom_->get_vertex_n());
    geom_->add_vertex(to_->x, to_->y, 0);
    return 0;
}

int GLNSFontLoader::line_to(FT_Vector* to_, Geometry* geom_)
{
    geom_->add_mesh_idx(geom_->get_vertex_n());
    geom_->add_vertex(to_->x, to_->y, 0);
    return 0;
}

int GLNSFontLoader::conic_to(FT_Vector* ctrl_, FT_Vector* to_, Geometry* geom_)
{
    Vec3 from = geom_->get_vertex(geom_->get_vertex_n() - 1);

    for (Size32 i = 0; 4 > i; ++i)
    {
        Ratio t = 0.25 * i;
        Vec3 ctrl(ctrl_->x, ctrl_->y, 0), to(to_->x, to_->y, 0);
        Vec3 result = from * pow(1-t, 2) + ctrl * 2 * t * (1-t) + to * pow(t, 2);
        geom_->add_mesh_idx(geom_->get_vertex_n());
        geom_->add_vertex(result.get_x(), result.get_y(), result.get_z());
    }

    return 0;
}

int GLNSFontLoader::cubic_to(FT_Vector* ctrl1_, FT_Vector* ctrl2_,
                              FT_Vector* to_, Geometry* geom_)
{
    Vec3 from = geom_->get_vertex(geom_->get_vertex_n() - 1);

    for (Size32 i = 0; 4 > i; ++i)
    {
        Ratio t = 0.25 * i;
        Vec3  ctrl1(ctrl1_->x, ctrl1_->y, 0), ctrl2(ctrl2_->x, ctrl2_->y, 0),
            to(to_->x, to_->y, 0);
        Vec3 result = from * pow(1-t, 3) + ctrl1 * 3 * t * pow(1-t, 2)
            + ctrl2 * 3* pow(t, 2) * (1-t) + to * pow(t, 3);
        geom_->add_mesh_idx(geom_->get_vertex_n());
        geom_->add_vertex(result.get_x(), result.get_y(), 0);
    }

    return 0;
}

#endif
