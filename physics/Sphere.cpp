#include "Common.hpp"

#include "Sphere.hpp"

#include "physics/Ray.hpp"
#include "physics/Tris.hpp"
#include "physics/SphereTree.hpp"

using namespace std;
using namespace reprize;
using namespace phy;

Sphere::Sphere(Unit radius_)
    : res::Body("Sphere"), radius(radius_)
{
}

Sphere::~Sphere(void)
{
}

const bool Sphere::is_collision(res::Body* cldr_, const Mtx44& rel_mtx_)
{
    return cldr_->is_collision(this, rel_mtx_);
}

const bool Sphere::is_collision(Sphere* cldr_, const Mtx44& rel_mtx_)
{

    if ((rel_mtx_.get_factor(12) * rel_mtx_.get_factor(12)
//         * rel_mtx_.get_factor(0) // x scale
            ) + (rel_mtx_.get_factor(13) * rel_mtx_.get_factor(13)
//         * rel_mtx_.get_factor(5) // y scale
            ) + (rel_mtx_.get_factor(14) * rel_mtx_.get_factor(14)
//         * rel_mtx_.get_factor(10) // z scale
            )
        < (cldr_->get_radius() + radius) * (cldr_->get_radius() + radius))
    { return true; }

    return false;
}

const bool Sphere::is_collision(Ray* cldr_, const Mtx44& rel_mtx_)
{
    return false;
}

const bool Sphere::is_collision(Tris* cldr_, const Mtx44& rel_mtx_)
{
    return false;
}

const bool Sphere::is_collision(SphereTree* cldr_, const Mtx44& rel_mtx_)
{
    return cldr_->is_collision(this, rel_mtx_);
}
