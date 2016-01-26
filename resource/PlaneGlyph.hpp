#ifndef PLANE_GLYPH_HPP_
#define PLANE_GLYPH_HPP_

#include "Glyph.hpp"

namespace reprize
{
namespace res
{
class Material;
class Model;
class Mesh;

class PlaneGlyph : public Glyph
{
public:
    PlaneGlyph(const Material* mtl_, const Int32 x_, const Int32 y_,
               const uInt32 width_, const uInt32 height_);
    virtual ~PlaneGlyph(void);

    Geometry* create_plane_glyph(const Vec3& topleft_,
                                 const Vec3& bottomright,
                                 const Vec3& uv_topleft,
                                 const Vec3& uv_bottomright);

protected:
};
}
}
#endif
