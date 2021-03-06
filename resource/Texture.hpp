#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include "Node.hpp"

namespace reprize
{
namespace res
{
class Texture : public Node
{
public:
    Texture(const Str name_, const uInt32 id_,
            const Int32 width_, const Int32 height_, const bool alpha_,
            const bool fallback_, const bool readonly_ = true)
        : Node(name_, NULL, readonly_), id(id_), width(width_),
          height(height_), fallback(fallback_), alpha(alpha_) {}
    Texture(const Texture& src_)
        : Node(src_), id(src_.id), width(src_.width),
          height(src_.height), fallback(src_.fallback), alpha(src_.alpha) {}

    virtual ~Texture(void) {}

    Props* gen_props(void)
        {
            RE_PROPS_START(Texture);
            RE_PROP_STR(Texture, uv);
            RE_PROP_STR(Texture, size);

            return RE_PROPS_FINISH;
        }
    virtual Texture* Clone(const bool with_child_ = true)
        { return this; }

    inline const uInt32 get_id(void) const { return id; }
    Int32 get_width(void) const { return width; }
    Int32 get_height(void) const { return height; }
    const bool has_alpha(void) const { return alpha; }

protected:
    const Str& prop_uv(const Str& arg_) { return RE_FAILURE; }
    const Str& prop_size(const Str& arg_) { return RE_FAILURE; }

    const uInt32 id;
    Int32 width, height;
    const bool fallback, alpha;
};
}
}

#endif
