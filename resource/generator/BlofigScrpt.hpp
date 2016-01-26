#ifndef BLOFIG_SCRPT_HPP_
#define BLOFIG_SCRPT_HPP_

#include "Common.hpp"
#include "PrimitiveScrpt.hpp"

using namespace reprize;
using namespace res;
using namespace std;

namespace  reprize
{
class Blofig : public res::Script
{
public:
    BlofigScript(Node* parent_) : Script(Str("Blofig"), parent_), missing("/")
        {
        }
    virtual ~Blofig(void)
        {
        }

    virtual const NID apply(const Str& txt_, const StrMap& am_)
        {
/*
            Str name = ATT(am_, "name");
            Str offset = ATT(am_, "offset");
            std::stringstream ln(offset);
            Unit x = 0, y = 0, z = 0, w = 1, h = 1, d = 1;

            RE_DBG("blofig though");

            NID actor_nid = bldr_->actor(name, x, y, z, w, h, d);
            bldr_->set_parent(actor_name);
            bldr_->push_parent();
            PrimitiveScrpt prim;
            Str mdl_name = bldr_->model(Str("boid_mdl"),
                                        Str("none"), Str("none"));
            bldr_->set_parent(mdl_name);

//             res::Geometry* geom = prim.build_box(Str("blofig"), FACE_FRONT,
//                                                  Vec3(30, 30, 30));
//             res::Geometry* geom = prim.build_plane(Str("blofig"),
//                                                    FACE_FRONT,
//                                                    30, 30, 10, 10);
            res::Geometry* geom = prim.build_stripe(Str("blofig"), FACE_FRONT,
                                                    Vec3(30, 30, 30));

//     res::Geometry* geom = prim.build_ico(Str("boid_geom"), FACE_FRONT, 3, 3);
            bldr_->geometry(geom);

            bldr_->pop_parent();
            Str head_name = bldr_->actor(name, 10, 0, -10, w, h, d);
            bldr_->set_parent(head_name);
            Str head_mdl_name = bldr_->model(Str("head_mdl"),
                                             Str("none"), Str("none"));
            bldr_->set_parent(head_mdl_name);

            res::Geometry* head_geom = prim.build_box(Str("terrain_geom"),
                                                 FACE_FRONT, Vec3(3, 3, 3));
//     res::Geometry* geom = prim.build_ico(Str("boid_geom"), FACE_FRONT, 3, 3);
            bldr_->geometry(head_geom);

//             return name;//bldr_->actor(name, x, y, z, w, h, d);
            return actor_nid;
*/
            return 0;
        }

private:
    Str actor_name;
    res::Geometry* geom;
//     Limb r_arm, l_arm, r_leg, l_leg;
//     Str head;
};

class Limb
{
public:
    Limb(void)
        {
        }
    virtual ~Limb(void)
        {
        }
    void init(Vec3& pos_, Vec3& rot_)
        {
        }
};
}

#endif
