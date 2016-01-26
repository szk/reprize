#include "Common.hpp"

#include "Tris.hpp"

#include "physics/Sphere.hpp"
#include "physics/Ray.hpp"
#include "physics/SphereTree.hpp"

using namespace reprize;
using namespace phy;

Tris::Tris(const Vec3& v1_, const Vec3& v2_,const Vec3& v3_)
    : Body(), v1(v1_), v2(v2_), v3(v3_)
{
}

Tris::~Tris(void)
{
}

const bool Tris::is_collision(res::Body* cldr_, const Mtx44& rel_mtx_)
{
    return false;
}

const bool Tris::is_collision(Sphere* cldr_, const Mtx44& rel_mtx_)
{
    return false;
}

const bool Tris::is_collision(Ray* cldr_, const Mtx44& rel_mtx_)
{
    return false;
}

const bool Tris::is_collision(Tris* cldr_, const Mtx44& rel_mtx_)
{
    return false;
}

const bool Tris::is_collision(SphereTree* cldr_, const Mtx44& rel_mtx_)
{
    return false;
}
