#include "Common.hpp"

#include "resource/Geometry.hpp"
#include "SphereTree.hpp"

#include "physics/Sphere.hpp"
#include "physics/Ray.hpp"
#include "physics/Tris.hpp"

using namespace reprize;
using namespace res;
using namespace phy;

SphereTree::SphereTree(const Vec3& v1_, const Vec3& v2_, const Vec3& v3_)
    : is_leaf(true), radius(0)
{
    center = (v1_ + v2_ + v3_) / 3;

    Vec3 v1 = center - v1_;
    Vec3 v2 = center - v2_;
    Vec3 v3 = center - v3_;

    const Unit v[] = { fabs(v1.get_x()), fabs(v2.get_x()), fabs(v3.get_x()),
                       fabs(v1.get_y()), fabs(v2.get_y()), fabs(v3.get_y()),
                       fabs(v1.get_z()), fabs(v2.get_z()), fabs(v3.get_z()),};
    for (Size32 i = 0; i < 9; ++i)
    {
        if (v[i] > radius) { radius = v[i]; }
    }
}

// node
SphereTree::SphereTree(SphereTree* child1_, SphereTree* child2_)
    : Body("SphereTree"), right(NULL), left(NULL), is_leaf(false), radius(0)
//       x((child1_->x + child2_->x) / 2),
//       y((child1_->y + child2_->y) / 2),
//       z((child1_->z + child2_->z) / 2)
{
//     Unit spheres[] = { fabs(child1_->x - x) + child1_->radius,
//                        fabs(child2_->x - x) + child2_->radius,
//                        fabs(child1_->y - y) + child1_->radius,
//                        fabs(child2_->y - y) + child2_->radius,
//                        fabs(child1_->z - z) + child1_->radius,
//                        fabs(child2_->z - z) + child2_->radius};
//     for (Size32 i = 0; i < 6; ++i)
//     {
//         if (spheres[i] > radius) { radius = spheres[i]; }
//     }
    right = child1_;
    left = child2_;
}

SphereTree::SphereTree(Str name_, Geometry* src_)
    : Body(name_), is_leaf(false), radius(0)
{
    if (src_ == NULL) { return; }

    for (Size32 m_idx = 0; src_->get_mesh_size() > m_idx; ++m_idx)
    {
        const Mesh* mesh = src_->get_mesh(m_idx);

        switch (mesh->get_prm())
        {
		case res::POINTS: break;
        case res::LINES: break;
        case res::LINE_LOOP: break;
        case res::LINE_STRIP: break;
        case res::TRIANGLES:
            bd_ls.push_back(new SphereTree(src_->get_vertex(mesh->get_idx(0)),
                                        src_->get_vertex(mesh->get_idx(1)),
                                        src_->get_vertex(mesh->get_idx(2))));

            if (bd_ls.back()->get_edge() > radius)
            { radius = bd_ls.back()->get_edge(); }
            break;
        case res::TRIANGLE_STRIP:
            for (Size32 v_idx = 0; mesh->get_idx_size() > v_idx; ++v_idx)
            {
//                 bd_ls.push_back(new SphereTree(
//                                      src_->get_vertex(mesh->get_idx(v_idx))));
            }
            break;
        case res::TRIANGLE_FAN: break;
        case res::QUADS: break;
        case res::QUAD_STRIP: break;
        case res::POLYGON: break;
        case res::PRIM_NUM: break;
        }
    }

//     RE_DBG("radius: %f", radius);
//     RE_DBG("Triangles: %f", bd_ls.size());
}

SphereTree::~SphereTree(void)
{
}

const bool SphereTree::is_collision(res::Body* cldr_, const Mtx44& rel_mtx_)
{
    return cldr_->is_collision(this, rel_mtx_);
}

const bool SphereTree::is_collision(Sphere* cldr_, const Mtx44& rel_mtx_)
{
    if (is_leaf)
    {
        if (((rel_mtx_.get_factor(12)   + center.get_x())
             * (rel_mtx_.get_factor(12) + center.get_x())) +
            ((rel_mtx_.get_factor(13)   + center.get_y())
             * (rel_mtx_.get_factor(13) + center.get_y())) +
            ((rel_mtx_.get_factor(14)   + center.get_z())
             * (rel_mtx_.get_factor(14) + center.get_z()))
            < (cldr_->get_radius() + radius) * (cldr_->get_radius() + radius))
        { return true; }
    }
    else if ((rel_mtx_.get_factor(12) * rel_mtx_.get_factor(12)) +
             (rel_mtx_.get_factor(13) * rel_mtx_.get_factor(13)) +
             (rel_mtx_.get_factor(14) * rel_mtx_.get_factor(14))
             < (cldr_->get_radius() + radius) * (cldr_->get_radius() + radius))
    {
        return true;
        for (std::vector<SphereTree*>::iterator itr = bd_ls.begin();
             itr != bd_ls.end(); ++itr)
        { return (*itr)->is_collision(cldr_, rel_mtx_); }
    }

    return false;
}

const bool SphereTree::is_collision(Ray* cldr_, const Mtx44& rel_mtx_)
{
    if (cldr_->is_collision(this, rel_mtx_))
    {
        for (std::vector<SphereTree*>::iterator itr = bd_ls.begin();
             itr != bd_ls.end(); ++itr)
        {
            Mtx44 c_mtx = rel_mtx_;
            Mtx44 center((*itr)->get_center() * -1);

            c_mtx.mlt(center);
            if (cldr_->is_collision((*itr), c_mtx)) { return true; }
        }
    }
    return false;
}

const bool SphereTree::is_collision(Tris* cldr_, const Mtx44& rel_mtx_)
{
//     RE_DBG("tris %f", radius);
    return false;
}

const bool SphereTree::is_collision(SphereTree* cldr_, const Mtx44& rel_mtx_)
{
//     RE_DBG("bdtree %f", radius);
    return false;
}

const Unit SphereTree::get_edge(void)
{
    Unit edge = fabs(center.get_x());
    if (fabs(center.get_y()) > edge) { edge = fabs(center.get_y()); }
    if (fabs(center.get_z()) > edge) { edge = fabs(center.get_z()); }

    return edge + radius;
}

const Unit SphereTree::get_radius(void)
{
    return radius;
}

const Vec3& SphereTree::get_center(void)
{
    return center;
}
