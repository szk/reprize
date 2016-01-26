#include "Common.hpp"

#include "Material.hpp"
#include "Glyph.hpp"
#include "Geometry.hpp"

#include "Model.hpp"

#include "PlaneGlyph.hpp"

using namespace reprize;
using namespace res;
using namespace std;

PlaneGlyph::PlaneGlyph(const Material* mtl_, const Int32 x_, const Int32 y_,
                       const uInt32 width_, const uInt32 height_)
    : Glyph(width_, height_)
{
    Vec3 topleft(0, 0);
    Vec3 bottomright(width, height);

    Vec3 uv_topleft = Vec3(static_cast<Unit>(x_), static_cast<Unit>(y_),
                           0) / mtl_->get_size();
    Vec3 uv_bottomright = Vec3(width + x_, height + y_, 0) / mtl_->get_size();
    model = new Model("glyph",
                      create_plane_glyph(topleft, bottomright, uv_topleft,
                                         uv_bottomright),
                      mtl_, BB_INDEPEND, AU_TRANSPARENCY);
}

PlaneGlyph::~PlaneGlyph(void)
{}

Geometry* PlaneGlyph::create_plane_glyph(const Vec3& topleft_,
                                         const Vec3& bottomright_,
                                         const Vec3& uv_topleft_,
                                         const Vec3& uv_bottomright_)
{
    Geometry* geometry = new Geometry("Glyph");

    Unit left = topleft_.get_x();
    Unit right = bottomright_.get_x();
    Unit top = topleft_.get_y();
    Unit bottom = bottomright_.get_y();

    Unit uv_left = uv_topleft_.get_x();
    Unit uv_right = uv_bottomright_.get_x();
    Unit uv_top = uv_topleft_.get_y();
    Unit uv_bottom = uv_bottomright_.get_y();

    geometry->add_mesh(TRIANGLE_STRIP);
    for (uInt32 i = 0; 4 > i; ++i) { geometry->add_mesh_idx(i); }

    geometry->add_vertex(left, top, 0, uv_left, uv_top);
    geometry->add_vertex(left, bottom, 0, uv_left, uv_bottom);
    geometry->add_vertex(right, top, 0, uv_right, uv_top);
    geometry->add_vertex(right, bottom, 0, uv_right, uv_bottom);

    return geometry;
}
