#ifndef BD_TREE_HPP_
#define BD_TREE_HPP_

#include "resource/Body.hpp"

namespace reprize
{
namespace res { class Geometry; }

namespace phy
{
class Sphere;
class Ray;
class Tris;

class SphereTree : public res::Body
{
public:
    // leaf
    SphereTree(const Vec3& v1_, const Vec3& v2_, const Vec3& v3_);
    // node
    SphereTree(SphereTree* child1_, SphereTree* child2_);
    // root
    SphereTree(Str name_, res::Geometry* src_ = NULL);
    virtual ~SphereTree(void);

    const bool is_collision(res::Body* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(Sphere* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(Ray* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(Tris* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(SphereTree* cldr_, const Mtx44& rel_mtx_);

    const Unit get_edge(void);
    const Unit get_radius(void);
    const Vec3& get_center(void);

protected:
    SphereTree* right,* left;
    const bool is_leaf;
    Vec3 center;
    Unit radius;

    std::vector<SphereTree*> bd_ls;
    Mtx44 pre_mtx;
};
}
}

#endif
