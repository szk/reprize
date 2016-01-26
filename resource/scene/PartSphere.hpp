#ifndef PARTSPHERE_HPP_
#define PARTSPHERE_HPP_

#include "resource/Entity.hpp"
#include "util/ImprovedNoise.hpp"

namespace reprize
{
namespace res
{
class PartSphere : public res::Entity
{
public:
    PartSphere(const Str name_, const Vec3& base_color_, const Size32 i_)
        : Entity(name_, new res::Matter), idx(0), ps_idx(i_), radius(0),
          color(base_color_)
        {
        }
    virtual ~PartSphere(void)
        {
        }

    const bool init(const Vec3& a_, const Vec3& b_, const Vec3& c_,
                    const Unit radius_, const Unit tri_len_,
                    res::PrmMode meshmode_)
        {
            radius = radius_;
            res::Geometry* geom = new res::Geometry("part_sphere_geom");
            mtt->set_model(new res::Model(Str("part_sphere_mdl"), geom));

            add_vert(geom, a_, b_, c_, tri_len_, meshmode_);

            return true;
        }

    void eval(const res::Matter* parent_mtt_) { Entity::eval(parent_mtt_); }

    void set_sibling(PartSphere* neg_, PartSphere* pos_, PartSphere* ab_)
        { neg_sibling = neg_; pos_sibling = pos_; ab_sibling = ab_; }
    PartSphere* get_neg_sibling(void) { return neg_sibling; }
    PartSphere* get_pos_sibling(void) { return pos_sibling; }
    PartSphere* get_ab_sibling(void) { return ab_sibling; }

    Size32 get_ps_idx(void) { return ps_idx; }

private:
    void add_vert(res::Geometry* geom_,
                  const Vec3& a_, const Vec3& b_, const Vec3& c_,
                  const Unit edge_len_, res::PrmMode meshmode_)
        {
            if ((((a_ - b_).length()) < edge_len_) &&
                (((b_ - c_).length()) < edge_len_) &&
                (((c_ - a_).length()) < edge_len_))
            {
                add_ico_vert(geom_, a_, b_, c_, meshmode_);
                return;
            }

            const Vec3 ab = get_mid_on_sphere(a_, b_);
            const Vec3 bc = get_mid_on_sphere(b_, c_);
            const Vec3 ca = get_mid_on_sphere(c_, a_);

            add_vert(geom_, a_, ab, ca, edge_len_, meshmode_);
            add_vert(geom_, ab, b_, bc, edge_len_, meshmode_);
            add_vert(geom_, ca, bc, c_, edge_len_, meshmode_);
            add_vert(geom_, ab, bc, ca, edge_len_, meshmode_);
        }

    inline Vec3 get_mid_on_sphere(const Vec3& a_, const Vec3& b_) const
        {
            Vec3 midpoint = (a_ + b_) * 0.5f;
            midpoint.normalize();
            return midpoint * radius;
        }

    void add_ico_vert(res::Geometry* geom_,
                      const Vec3& a_, const Vec3& b_, const Vec3& c_,
                      res::PrmMode meshmode_)
        {
            geom_->add_mesh(meshmode_);

            Unit a_n = static_cast<Unit>(
                inoise.noise(static_cast<Float64>(a_.get_x()),
                             static_cast<Float64>(a_.get_y()),
                             static_cast<Float64>(a_.get_z())));
            Unit b_n = static_cast<Unit>(
                inoise.noise(static_cast<Float64>(b_.get_x()),
                             static_cast<Float64>(b_.get_y()),
                             static_cast<Float64>(b_.get_z())));
            Unit c_n = static_cast<Unit>(
                inoise.noise(static_cast<Float64>(c_.get_x()),
                             static_cast<Float64>(c_.get_y()),
                             static_cast<Float64>(c_.get_z())));

            Vec3 bump_a = a_; bump_a *= (0.01f * static_cast<Unit>(a_n)) + 1;
            Vec3 bump_b = b_; bump_b *= (0.01f * static_cast<Unit>(b_n)) + 1;
            Vec3 bump_c = c_; bump_c *= (0.01f * static_cast<Unit>(c_n)) + 1;

            Vec3 col;
            col.set(c_n, c_n, c_n);
//             col.set((float)idx / 12.0, (float)idx / 12.0,
//                     (float)idx / 12.0);
            col *= color;
            geom_->add_vertex(bump_c, 0, 0, col);
            geom_->add_mesh_idx(idx++);

            col.set(b_n, b_n, b_n);
//             col.set((float)idx / 12.0, (float)idx / 12.0,
//                     (float)idx / 12.0);
            col *= color;
            geom_->add_vertex(bump_b, 0, 0, col);
            geom_->add_mesh_idx(idx++);

            col.set(a_n, a_n, a_n);
//             col.set((float)idx / 12.0, (float)idx / 12.0,
//                     (float)idx / 12.0);
            col *= color;
            geom_->add_vertex(bump_a, 0, 0, col);
            geom_->add_mesh_idx(idx++);
        }

    Size32 idx, ps_idx;
    Unit radius;
    Vec3 color;
    ImprovedNoise inoise;
    PartSphere* neg_sibling,* pos_sibling,* ab_sibling;
};
}
}

#endif
