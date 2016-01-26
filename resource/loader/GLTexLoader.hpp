#ifndef GL_TEX_LOADER_HPP_
#define GL_TEX_LOADER_HPP_

#include "TexLoader.hpp"

namespace reprize
{
namespace res
{
class GLTexLoader : public TexLoader
{
public:
    GLTexLoader(void);
    virtual ~GLTexLoader(void);

    const bool load(const Str& filename_, const Str& path_);

    Texture* build(const Str& name_, const uInt64* array_,
                  const uInt32 breadth_, const Vec3& fg_, const Vec3& bg_,
                  const Ratio fg_op_ = 1.0, const Ratio bg_op_ = 1.0,
                  const MinFilter min_ = LINEAR_MIPMAP_LINEAR,
                  const MagFilter mag_ = LINEAR);

    Texture* build(const Str& name_, const uInt32* array_,
                  const uInt32 w_pow_,
                  const MinFilter min_ = LINEAR_MIPMAP_LINEAR,
                  const MagFilter mag_ = LINEAR);

protected:
    const GLuint load_png(const Str& path_, const Str& filename_,
                          Int32 gl_min_filter_, Int32 gl_mag_filter_);
    const GLuint load_vert(const Str& path_, const Str& filename_);
    const GLuint load_frag(const Str& path_, const Str& filename_);
    const GLuint gen_texture(const GLubyte* texel_,
                             const uInt32 width_, const uInt32 height_,
                             const MinFilter min_ = LINEAR_MIPMAP_LINEAR,
                             const MagFilter mag_ = LINEAR);

    const bool test_texture(const uInt32 width_, const uInt32 height_,
                            const MinFilter min_ = LINEAR_MIPMAP_LINEAR,
                            const MagFilter mag_ = LINEAR);
    GLuint gl_min(MinFilter min_);
    GLuint gl_mag(MagFilter mag_);
    const bool print_lasterror(void);
};
}
}
#endif
