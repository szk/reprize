#ifndef ACTOR_HPP_
#define ACTOR_HPP_

#include "Entity.hpp"

namespace reprize
{

namespace res
{
class Cell;
}

namespace res
{
class Script;

class Actor : public Entity
{
public:
    Actor(Str name_, Model* mdl_ = NULL, Body* body_ = NULL,
          Script* script_ = NULL);
    Actor(const Actor& src_);
    virtual ~Actor(void);

    virtual Actor* Clone(const bool with_child_ = true);
    Body* get_body(void);
    void update_body(void);

    void set_collider(Actor* actor_);

    virtual void eval(const Matter* parent_mtt_);
    Props* gen_props(void);

protected:
    Body* body;
    std::list<Actor*>collider;
    res::Cell* here;
    Script* script;

protected:
    virtual const bool Adopt(Node* node_);

    const Str& prop_model(Node* arg_);
    const Str& prop_forward(const Str& arg_);
    const Str& prop_back(const Str& arg_);
    const Str& prop_right(const Str& arg_);
    const Str& prop_left(const Str& arg_);
    const Str& prop_up(const Str& arg_);
    const Str& prop_down(const Str& arg_);
    const Str& prop_attack(const Str& arg_);
    const Str& prop_all(const Str& arg_);

    const Str& prop_pos(const Str& arg_);
    const Str& prop_quat(const Str& arg_);
    const Str& prop_color(const Str& arg_);

    Vec3 dst_trans;
    Unit step;

    Str def_arg;
    // state
    bool attack_toggle;
    KeyframeCommand kf_cmd;
};
}
}

#endif
