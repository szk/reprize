#ifndef TEX_LOADER_HPP_
#define TEX_LOADER_HPP_

#include "resource/TexAtlas.hpp"
#include "resource/Texture.hpp" // It can't use 'enum' at previous definition
#include "Loader.hpp"

namespace reprize
{
namespace res
{
class TexLoader : public Loader
{
public:
    TexLoader(void) : Loader("TextureLoader"), def_tex(NULL) {}
    virtual ~TexLoader(void) {}

    virtual Texture* get_def_texture(void)
        {
            if (def_tex == NULL)
            {
                def_tex = build(Str("Default"), def::def_bit,
                                def::def_breadth, Vec3(255, 255, 255),
                                Vec3(128, 128, 128), 1, 0.5);
            }
            return def_tex;
        }

    // for superclass
    Node* create_sys_node(void)
        {
            Vec3 fg(255, 255, 255), bg(0, 0, 0);
            Node* sys = new Node("Texture");
            sys->Add(build(Str("White"), def::filled_bit,
                              def::filled_breadth, fg, bg, 1, 0));
            sys->Add(build(Str("Black"), def::filled_bit,
                              def::filled_breadth, bg, bg, 1, 0));
            sys->Add(build(Str("Transparent"), def::filled_bit,
                              def::filled_breadth, fg, bg, 0, 0));
            sys->Add(build(Str("Arrow"), def::arrow_bit, def::arrow_breadth,
                              fg, bg, 1, 0, NEAREST_MIPMAP_NEAREST, NEAREST));
            sys->Add(build(Str("Beam"), def::beam_bit, def::beam_breadth,
                              fg, bg, 1, 0, NEAREST_MIPMAP_NEAREST, NEAREST));

            fg.set(64, 64, 64), bg.set(128, 128, 128);
            sys->Add(build(Str("Button"), def::button_bit, def::button_breadth,
                              fg, bg, 1, 0, NEAREST_MIPMAP_NEAREST, NEAREST));
            sys->Add(build(Str("Thumb"), def::thumb_bit, def::thumb_breadth,
                           fg, bg, 1, 0, NEAREST_MIPMAP_NEAREST, NEAREST));
            sys->Add(build(Str("Frame"), def::frame_bit, def::frame_breadth,
                           fg, bg, 0, 1, NEAREST_MIPMAP_NEAREST, NEAREST));
            sys->Add(build(Str("Border"), def::border_bit, def::border_breadth,
                           fg, bg, 1, 0, NEAREST_MIPMAP_NEAREST, NEAREST));
            sys->Add(def_tex);
            return sys;
        }
    virtual const bool load(const Str& filename_, const Str& path_) = 0;
    virtual Texture* build(const Str& name_, const uInt64* array_,
                           const uInt32 breadth_,
                           const Vec3& fg_, const Vec3& bg_,
                           const Ratio fg_op_ = 1.0, const Ratio bg_op_ = 1.0,
                           const MinFilter min_ = LINEAR_MIPMAP_LINEAR,
                           const MagFilter mag_ = LINEAR) = 0;
    virtual Texture* build(const Str& name_, const uInt32* array_,
                           const uInt32 w_pow_,
                           const MinFilter min_ = LINEAR_MIPMAP_LINEAR,
                           const MagFilter mag_ = LINEAR) = 0;

protected:
    inline bool is_pot(const uInt32 num_)
        {
            if (num_ == 0) { return false; }
            uInt32 val = num_;
            while ((val & 1) == 0) { val >>= 1; }
            return val == 1;
        }
    Texture* def_tex;
};
}
}
#endif
