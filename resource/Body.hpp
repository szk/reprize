#ifndef BODY_HPP_
#define BODY_HPP_

#include "Node.hpp"

namespace reprize
{
namespace phy { class Sphere; class Ray; class Tris; class SphereTree; }

namespace res
{
class Geometry;

class Body : public Node
{
public:
    virtual ~Body(void) {}

    virtual const bool is_collision(res::Body*, const Mtx44&) = 0;
    virtual const bool is_collision(phy::Sphere*, const Mtx44&) = 0;
    virtual const bool is_collision(phy::Ray*, const Mtx44&) = 0;
    virtual const bool is_collision(phy::Tris*, const Mtx44&) = 0;
    virtual const bool is_collision(phy::SphereTree*, const Mtx44&) = 0;

protected:
    Body(Str name_, Geometry* src_ = NULL)
        : Node(name_), pinned(false)
        {}
    Body(void)
        : Node("body")
        {}

    Actor* last_collide;
    bool pinned;
};
}
}

#endif

