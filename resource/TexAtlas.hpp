#ifndef TEX_ATLAS_HPP_
#define TEX_ATLAS_HPP_

#include "Node.hpp"

namespace reprize
{
namespace res
{
enum MinFilter
{
    NEAREST_MIPMAP_NEAREST, LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_LINEAR,
    MINFILTER_NUM,
};

enum MagFilter { NEAREST, LINEAR, MAGFILTER_NUM, };

class TexAtlas : public Node
{
public:
    TexAtlas(const Str name_, const uInt32 id_,
            const Int32 width_, const Int32 height_, const bool alpha_,
            const bool fallback_, const bool readonly_ = true)
        : Node(name_, NULL, readonly_), id(id_), width(width_),
          height(height_), fallback(fallback_), alpha(alpha_) {}
    TexAtlas(const TexAtlas& src_)
        : Node(src_), id(src_.id), width(src_.width),
          height(src_.height), fallback(src_.fallback), alpha(src_.alpha) {}

    virtual ~TexAtlas(void) {}

    virtual TexAtlas* Clone(const bool with_child_ = true) { return this; }

    inline const uInt32 get_id(void) const { return id; }
    Int32 get_width(void) const { return width; }
    Int32 get_height(void) const { return height; }
    const bool has_alpha(void) const { return alpha; }

protected:
    const uInt32 id;
    Int32 width, height;
    const bool fallback, alpha;
};
}
}

#endif
