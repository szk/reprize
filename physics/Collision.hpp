#ifndef COLLISION_HPP_
#define COLLISION_HPP_

#include "resource/Actor.hpp"
#include "resource/Body.hpp"

#include "physics/Sphere.hpp"
#include "physics/Ray.hpp"
#include "physics/Tris.hpp"
#include "physics/SphereTree.hpp"

namespace reprize
{
namespace phy
{
typedef res::PropStr<Collision> CollPropStr;

class Collision : public res::Node
{
public:
    Collision(void) : Node("Collision"), dynamic(NULL), base(NULL) {}
    virtual ~Collision(void) { }

    virtual void init(void)
        {
            res::Props* pr = new res::Props;
            pr->Add("showboundingvolume",
                    new CollPropStr(this, &Collision::prop_showbv));
        }

    void set_view(inf::Interface* inf_) { inf = inf_; }

    virtual void add_table(res::Actor* actor_, const bool base_)
        {
            if (actor_->get_body()) { body_table.push_back(actor_); }
        }

    virtual void tick(void)
        {
        }
    virtual void check(void)
        {
            Vec3 col_hit(0,1,1), col_no(1, 1, 1);

            std::vector<res::Actor*>::iterator current_itr, base_itr;
            Mtx44 base_mtx, current_mtx;
            res::Body* base_body,* current_body;

            uInt32 base = 0, current = 0;

            for (std::vector<res::Actor*>::iterator itr = body_table.begin();
                 itr != body_table.end(); ++itr)
            {
                (*itr)->set_color(col_no);
//                 inf->add_beacon((*itr)->get_mtt());
            }

            for (base_itr = body_table.begin(); base_itr != body_table.end();
                 ++base_itr)
            {
                ++base;

                base_body = (*base_itr)->get_body();
                base_mtx = (*base_itr)->get_mtt()->get_c_mtx();
                base_mtx.inverse();

                // Should use KD-Tree
                for (current_itr = base_itr; ++current_itr != body_table.end();)
                {
                    ++current;
                    current_body = (*current_itr)->get_body();
                    current_mtx = (*current_itr)->get_mtt()->get_c_mtx();

                    current_mtx.mlt(base_mtx);

                    if (current_body->is_collision(base_body, current_mtx))
                    {
                        (*current_itr)->set_collider((*base_itr));
                        (*base_itr)->set_collider((*current_itr));
                        (*current_itr)->set_color(col_hit);
                        (*base_itr)->set_color(col_hit);
                    }
                }
            }
        }
    virtual void finish(void)
        {
        }

    void set_actor(res::Actor* dynamic_, res::Actor* base_)
        {
            dynamic = dynamic_;
            base = base_;
        }
    const Str& prop_showbv(const Str& arg_)
        { show_bv = true; return RE_SUCCESS; }

protected:
    inf::Interface* inf;
    std::vector<res::Actor*> body_table;
    res::Model* bounding_area;
    res::Actor* dynamic,* base;

    bool show_bv;
};
}
}

#endif
