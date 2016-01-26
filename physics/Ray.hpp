#ifndef RAY_HPP_
#define RAY_HPP_

#include "resource/Body.hpp"

namespace reprize
{
namespace phy
{
class Sphere;
class Tris;
class SphereTree;

class Ray : public res::Body
{
public:
    Ray(void);
    virtual ~Ray(void);

    const bool is_collision(res::Body* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(Sphere* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(Ray* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(Tris* cldr_, const Mtx44& rel_mtx_);
    const bool is_collision(SphereTree* cldr_, const Mtx44& rel_mtx_);

protected:
};
}
}

#endif
