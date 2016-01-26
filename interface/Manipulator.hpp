#ifndef MANIPULATOR_HPP_
#define MANIPULATOR_HPP_

#include "resource/Geometry.hpp"
#include "resource/Body.hpp"
#include "resource/Actor.hpp"

namespace reprize
{
namespace inf
{
class Manipulator : public res::Actor
{
public:
    Manipulator(void) : Actor("Manipulator") {}
    virtual ~Manipulator(void) {}

    void init(const Vec3& size_)
        {
//             res::Model* arrow_mdl = new res::Model("Arrow", geom_arrow(size_));
//             res::Model* orb_mdl = new res::Model("Orb", geom_orb(size_));
//             res::Model* knob_mdl = new res::Model("Knob", geom_knob(size_));

//             for (Size32 i = 0; i < 3; ++i)
//             {
//                 arrow[i] = new res::Actor(arrow_mdl->get_name(), arrow_mdl);
//                 orb[i] = new res::Actor(orb_mdl->get_name(), orb_mdl);
//                 knob[i] = new res::Actor(knob_mdl->get_name(), knob_mdl);
//                 Add(arrow[i]);
//                 Add(orb[i]);
//                 Add(knob[i]);
//             }
        }

    void eval(const res::Matter* parent_mtt_)
        { res::Actor::eval(parent_mtt_); }

protected:
    res::Geometry* geom_arrow(const Vec3& size_)
        {
            res::Geometry* geom = new res::Geometry("Arrow",
                                                    new std::vector<res::Mesh*>,
                                                    new std::vector<Unit>,
                                                    new std::vector<Unit>,
                                                    new std::vector<Unit>,
                                                    new std::vector<Unit>);

            // axis
            geom->add_mesh(res::LINES);
            geom->add_mesh_idx(0); geom->add_mesh_idx(1);
            geom->add_vertex(0, 0, -20); geom->add_vertex(0, 0, 20);

            // cone
            geom->add_mesh(res::TRIANGLE_FAN);
            Size32 slice = 6;
            Unit r = 10;
            geom->add_mesh_idx(0);
            for (Size32 i = 0; slice > i; ++i)
            {
                geom->add_mesh_idx(i + 2);
                Unit x = sinf(static_cast<Unit>((PI * 2) * i / slice));
                Unit z = cosf(static_cast<Unit>((PI * 2) * i / slice));
                geom->add_vertex(x * r, z * r, 0.0);
            }
            geom->add_mesh_idx(2);

            // cap
            geom->add_mesh(res::TRIANGLE_FAN);
            for (Size32 i = 0; slice > i; ++i)
            { geom->add_mesh_idx(i + 2); }

            return geom;
        }
    res::Geometry* geom_orb(const Vec3& size_)
        {
            res::Geometry* geom = new res::Geometry("Orb",
                                                    new std::vector<res::Mesh*>,
                                                    new std::vector<Unit>,
                                                    new std::vector<Unit>,
                                                    new std::vector<Unit>,
                                                    new std::vector<Unit>);

            geom->add_mesh(res::TRIANGLE_FAN);
            Size32 slice = 12;
            Unit r = 10;
            for (Size32 i = 0; slice > i; ++i)
            {
                geom->add_mesh_idx(i);
                Unit x = sinf(static_cast<Unit>((PI * 2) * i / slice));
                Unit z = cosf(static_cast<Unit>((PI * 2) * i / slice));
                geom->add_vertex(x * r, z * r, 0.0);
            }

            return geom;
        }
    res::Geometry* geom_knob(const Vec3& size_)
        {
            res::Geometry* geom = new res::Geometry("Knob",
                                                    new std::vector<res::Mesh*>,
                                                    new std::vector<Unit>,
                                                    new std::vector<Unit>,
                                                    new std::vector<Unit>,
                                                    new std::vector<Unit>);

            geom->add_mesh(res::TRIANGLE_STRIP);
            geom->add_mesh_idx(0); geom->add_mesh_idx(1); geom->add_mesh_idx(2);
            geom->add_mesh_idx(1); geom->add_mesh_idx(0);

            geom->add_vertex(1, 0, 0); geom->add_vertex(1, 1, 0);
            geom->add_vertex(0, 1, 1);

            return geom;
        }

    res::Actor* arrow[3],* orb[3],* knob[3];
    Vec3 view_pos, input_pos;
    Mtx44 ptr_mtx;
};
}
}
#endif
