#include "Common.hpp"

#include "Texture.hpp"
#include "Glyph.hpp"
#include "Geometry.hpp"

#include "Model.hpp"

#include "GeomGlyph.hpp"

using namespace reprize;
using namespace res;
using namespace std;

GeomGlyph::GeomGlyph(Geometry* geom_, const Int32 width_, const Int32 height_)
    : Glyph(width_, height_)
{
    model = new Model("glyph", geom_, NULL, BB_INDEPEND, AU_TRANSPARENCY);
}

GeomGlyph::~GeomGlyph(void)
{}

Geometry* GeomGlyph::create_geom_glyph(const Vec3& topleft_,
                                       const Vec3& bottomright,
                                       const Vec3& uv_topleft,
                                       const Vec3& uv_bottomright)
{ return NULL; }
