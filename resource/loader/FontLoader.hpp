#ifndef FONT_LOADER_HPP_
#define FONT_LOADER_HPP_

#include "Loader.hpp"
#include "../Font.hpp"
#include "../PlaneGlyph.hpp"
#include "../Material.hpp"

class EnvDepInfo;

namespace reprize
{
namespace res
{
class Texture;
class Font;
class Glyph;

class FontLoader : public Loader
{
public:
    FontLoader(EnvDepInfo* depinfo_)
        : Loader("FontLoader"), tex_loader(NULL), font(NULL) {}
    virtual ~FontLoader(void) {}

    void set_tex_loader(TexLoader* tex_loader_)
        { tex_loader = tex_loader_; }

    Node* create_sys_node(void)
        {
            Texture* regular_tex = tex_loader->
                build("ReFn", def::re_fn_bit, 128, Vec3(255, 255, 255),
                      Vec3(0, 0, 0), 1, 0, NEAREST_MIPMAP_NEAREST, NEAREST);
            Material* re_mtl = new Material("ReMtl");
            re_mtl->add_texture(regular_tex);

            Font* regular_fn = new Font(Str("ReFont"),
                                        static_cast<Unit>(def::re_fn_height),
                                        new PlaneGlyph(re_mtl, 0, 0, 8,
                                                       def::re_fn_height));

            for (Size32 i = 0; i < 97; ++i)
            {
                uInt32 x = def::re_fn_coord[i].x;
                uInt32 y = def::re_fn_coord[i].y;
                uInt32 width = def::re_fn_coord[i].width;
                Char ch = def::re_fn_coord[i].ch;
                regular_fn->add_glyph(new PlaneGlyph(re_mtl, x, y, width,
                                                     def::re_fn_height), ch);
            }

            Texture* micro_tex = tex_loader->
                build("MicroFn", def::micro_fn_bit, 64,
                      Vec3(255, 255, 255), Vec3(0, 0, 0), 1, 0,
                      NEAREST_MIPMAP_NEAREST, NEAREST);
            Material* micro_mtl = new Material("MicroMtl");
            micro_mtl->add_texture(micro_tex);

            Font* micro_fn = new Font(Str("MicroFont"),
                                      static_cast<Unit>(def::micro_fn_height),
                                      new PlaneGlyph(micro_mtl, 0, 0, 8,
                                                     def::micro_fn_height));

            for (Size32 i = 0; i < 128; ++i)
            {
                uInt32 x = (static_cast<uInt32>(i) % 16) * def::micro_fn_width;
                uInt32 y = (static_cast<uInt32>(i) / 16) * def::micro_fn_height;
                Char ch = static_cast<Char>(i);
                micro_fn->add_glyph(new PlaneGlyph(micro_mtl, x, y,
                                                   def::micro_fn_width,
                                                   def::micro_fn_height), ch);
            }

            Node* sys = new Node("Font");
            sys->Add(regular_fn);
            sys->Add(micro_fn);

            return sys;
        }
    virtual const bool load(const Str& filename_, const Str& path_)
        { return false; }

protected:
    virtual Glyph* make_tex(const Str& filename_, Int32 x_, Int32 y_,
                            Int32 width_, Int32 height_) { return NULL; }
    virtual Glyph* make_geom(const Str& filename_, uInt32 code_)
        { return NULL; }
    virtual Geometry* read_ttf(const Str& filename_, uInt32 code_)
        { return NULL; }
    TexLoader* tex_loader;
    Font* font;
};
}
}
#endif
