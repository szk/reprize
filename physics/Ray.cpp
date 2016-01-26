#include "Common.hpp"

#include "Ray.hpp"

#include "physics/Sphere.hpp"
#include "physics/Tris.hpp"
#include "physics/SphereTree.hpp"

using namespace reprize;
using namespace phy;

Ray::Ray(void) : res::Body("Ray")
{
}

Ray::~Ray(void)
{
}

const bool Ray::is_collision(res::Body* cldr_, const Mtx44& rel_mtx_)
{
    return cldr_->is_collision(this, rel_mtx_);
}

const bool Ray::is_collision(Sphere* cldr_, const Mtx44& rel_mtx_)
{
    Vec3 eye_pos(0, 0, 1);
    Quat q = rel_mtx_.get_quat();
//     q.get_angle(&angle, &eye_pos);
//     eye_pos *= angle;

    Vec3 col_pos = rel_mtx_.get_pos();

    Unit xc2 = col_pos.dot(col_pos);
    Unit vxc = col_pos.dot(eye_pos);
    Unit d = vxc * vxc - xc2 + cldr_->get_radius();

    if (d < 0) { return false;  }

    Unit tn = -vxc-sqrtf(d);
    Unit tp = -vxc+sqrtf(d);

    if (tn < 0 && tp <0) { return true; }
    return true;
}

const bool Ray::is_collision(Ray* cldr_, const Mtx44& rel_mtx_)
{
    RE_DBG("Ray");
    return false;
}

const bool Ray::is_collision(Tris* cldr_, const Mtx44& rel_mtx_)
{
    RE_DBG("Tris");
    return false;
}

const bool Ray::is_collision(SphereTree* cldr_, const Mtx44& rel_mtx_)
{
    Vec3 eye_pos = rel_mtx_.get_pos();

    Unit xc2 = eye_pos.dot(eye_pos);
    Unit vxc = eye_pos.dot(rel_mtx_.get_dir());
    Unit d = vxc * vxc - xc2 + (cldr_->get_radius() * cldr_->get_radius());

    if (d < 0) { return false;  }

    Unit tn = -vxc-sqrtf(d);
    Unit tp = -vxc+sqrtf(d);

    if (tn < 0 && tp < 0) { return true; }
    return false;
}
