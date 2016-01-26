#ifndef GEOM_GLYPH_HPP_
#define GEOM_GLYPH_HPP_

#include "Glyph.hpp"

namespace reprize
{
namespace res
{
class Geometry;
class Model;
class Mesh;

class GeomGlyph : public Glyph
{
public:
    GeomGlyph(Geometry* geom_, const Int32 width_, const Int32 height_);
    virtual ~GeomGlyph(void);

    Geometry* create_geom_glyph(const Vec3& topleft_,
                                const Vec3& bottomright,
                                const Vec3& uv_topleft,
                                const Vec3& uv_bottomright);

protected:
};
}
}
#endif
