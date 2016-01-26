#include "Common.hpp"

#include "Collision.hpp"
#include "Impulse.hpp"

// #include "BulletCollision.hpp"
// #include "BulletImpulse.hpp"

#include "resource/Keyframe.hpp"
#include "resource/Actor.hpp"

using namespace reprize;
using namespace res;
using namespace phy;

Physics::Physics(void)
    : node(NULL)
{
    pvt = new phy_pvt;
    node = new Node("Physics");
}

Physics::~Physics(void)
{
    DEL(pvt);
    DEL(node);
}

res::Node* Physics::get_node(void)
{
    return node;
}

const bool Physics::init(void)
{
//     Collision* collision = new BulletCollision();
    Collision* collision = new Collision();
    Impulse* impulse = new Impulse();
    collision->init();
    impulse->init();
    node->Add(collision);
    node->Add(impulse);

    set_law(collision, impulse);

    return true;
}

void Physics::set_view(inf::Interface* inf_)
{
    pvt->inf = inf_;
    pvt->collision->set_view(inf_);
}

void Physics::set_law(Collision* collision_, Impulse* impulse_)
{
    pvt->collision = collision_;
    pvt->impulse = impulse_;
}

void Physics::tick(void)
{
    pvt->collision->tick();
}

void Physics::add_actor(Actor* actor_, const bool base_)
{
    pvt->collision->add_table(actor_, base_);
    return;

    if (actor_) { pvt->inf->add_beacon(actor_); }

    for (actor_->First(); !actor_->IsDone(); actor_->Next())
    {
        Actor* child = dynamic_cast<Actor*>(actor_->Current());
        if (child) { add_actor(child, base_); }
    }
}
