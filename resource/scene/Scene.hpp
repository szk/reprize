#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "video/RenderState.hpp"
#include "Space.hpp"

namespace reprize
{
namespace res
{
class Scene : public Entity
{
public:
    Scene(const Str name_, Matter* mtt_ = NULL)
        : Entity(name_, mtt_), pov(NULL)
        {
            mtt = new vid::RenderState(new Model("scene_rs"),
                                       Mtx44(), Vec3(1.0, 1.0, 1.0), 1.0,
                                       Mtx44(), true,
                                       75, vid::SB_REVERSE,
                                       vid::RT_COLOR | vid::RT_ZBUFFER,
                                       RS_CLEAR_Z, RS_MASK_Z,
                                       RS_BF_SRC_ALPHA
                                       | (RS_BF_ONE_MINUS_SRC_ALPHA << 4));
        }
    Scene(const Scene& src_)
        : Entity(src_.name, NULL), pov(NULL)
        {
            mtt = new vid::RenderState(new Model("scene_rs"),
                                       Mtx44(), Vec3(1.0, 1.0, 1.0), 1.0,
                                       Mtx44(), true,
                                       75, vid::SB_REVERSE,
                                       vid::RT_COLOR | vid::RT_ZBUFFER,
                                       RS_CLEAR_Z, RS_MASK_Z,
                                       RS_BF_SRC_ALPHA
                                       | (RS_BF_ONE_MINUS_SRC_ALPHA << 4));
        }
    virtual ~Scene(void) {}

    res::Props* gen_props(void)
        {
            RE_PROPS_START(Scene);
            RE_PROP_NODE(Scene, pov);
            RE_PROP_STR(Scene, fov);
            RE_PROP_STR(Scene, color);
            return RE_PROPS_FINISH;
        }

    virtual Scene* Clone(const bool with_child_ = true)
        {
            Scene* current = new Scene(*this);
            for (First(); !IsDone(); Next())
            { current->Add(Current()->Clone(true)); }
            return current;
        }

    virtual void eval(const Matter* parent_mtt_)
        {
            if (pov) { prev_pov_pos = pov->get_rel_mtx().get_pos(); }
            Entity::eval(parent_mtt_);
        }

    const bool set_pov(Actor* pov_)
        {
            if ((pov = pov_))
            {
                for (First(); !IsDone(); Next())
                {
                    Space* current = dynamic_cast<Space*>(Current());
                    if (current) { current->set_pov(pov_); }
                }
                return true;
            }
            return false;
        }

    vid::RenderState* get_rs(void)
        { return dynamic_cast<vid::RenderState*>(mtt); }

    Actor* get_pov(void) const
        {
            // This method should get cell's renderstate
            // from the space which has a pov actor
            return pov;
        }

protected:
    const Str& prop_pov(Node* arg_)
        {
            res::Actor* loaded_actor = dynamic_cast<res::Actor*>(arg_);
            RE_DBG("NULL Avatar Prop");
            if (NULL == loaded_actor) { return RE_FAILURE; }
            pov = loaded_actor;
            RE_DBG("Avatar Prop: %s", loaded_actor->get_name().c_str());
            return RE_SUCCESS;
        }
    const Str& prop_fov(const Str& arg_)
        {
            return RE_SUCCESS;
        }
    const Str& prop_color(const Str& arg_)
        {
            if (dynamic_cast<vid::RenderState*>(mtt))
            {
                RE_DBG("%s has rs", name.c_str());
            }
            else
            {
                RE_DBG("%s does not have rs", name.c_str());
            }

            if (arg_.empty()) { return RE_FAILURE; }
            Vec3 vec(arg_);
            Entity::set_color(vec);

            return RE_SUCCESS;
        }

    Actor* pov;
    Vec3 prev_pov_pos;
    std::list<Space*> space_ls;

private:
    virtual const bool Adopt(Node* node_)
        {
            /*
            if (dynamic_cast<Space*>(node_))
            {
                RE_DBG("SET POV: %s", pov->get_name().c_str());

                dynamic_cast<Space*>(node_)->set_pov(pov);
//                 dynamic_cast<Space*>(node_)->set_audio(audio);
            }
            */
            return Entity::Adopt(node_);
        }
};
}
}

#endif
