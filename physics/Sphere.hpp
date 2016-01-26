#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "resource/Body.hpp"

namespace reprize
{
namespace phy
{
class Ray;
class Tris;
class SphereTree;

class Sphere : public res::Body
{
public:
    Sphere(Unit radius_ = 1);
    virtual ~Sphere(void);

    virtual const bool is_collision(res::Body* cldr_, const Mtx44& rel_mtx_);
    virtual const bool is_collision(phy::Sphere* cldr_, const Mtx44& rel_mtx_);
    virtual const bool is_collision(phy::Ray* cldr_, const Mtx44& rel_mtx_);
    virtual const bool is_collision(phy::Tris* cldr_, const Mtx44& rel_mtx_);
    virtual const bool is_collision(phy::SphereTree* cldr_, const Mtx44& rel_mtx_);

    inline Unit get_radius(void) { return radius; }

protected:
    Unit radius;
};
}
}

#endif
