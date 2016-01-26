#ifndef TRIS_HPP_
#define TRIS_HPP_

#include "resource/Body.hpp"

namespace reprize
{
namespace phy
{
class Sphere;
class Ray;
class SphereTree;

class Tris : public res::Body
{
public:
    Tris(const Vec3& v1_, const Vec3& v2_,const Vec3& v3_);
    virtual ~Tris(void);

    const bool is_collision(res::Body* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(Sphere* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(Ray* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(Tris* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(SphereTree* cldr_, const Mtx44& rel_mtx_);

protected:
    const Vec3 v1, v2, v3;
};
}
}

#endif
