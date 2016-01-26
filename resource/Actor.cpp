#include "Common.hpp"

#include "VFS.hpp"
#include "Body.hpp"
#include "Script.hpp"
#include "Actor.hpp"

#include "video/RenderState.hpp"

using namespace reprize;
using namespace std;
using namespace phy;
using namespace res;

Actor::Actor(Str name_, Model* mdl_, Body* body_, Script* script_)
    : Entity(name_, new Matter(mdl_)), body(body_), here(NULL), script(NULL),
      step(1.0), attack_toggle(false), kf_cmd(KF_CTRL_WAIT)
{
    prop_all("off");
}

Actor::Actor(const Actor& src_)
    : Entity(src_),
      body(src_.body), here(NULL), script(src_.script), step(1.0),
      attack_toggle(false), kf_cmd(KF_CTRL_WAIT)
{
    prop_all("off");
}

Actor::~Actor(void)
{
}

const bool Actor::Adopt(Node* node_)
{
    if (dynamic_cast<Body*>(node_)) { body = dynamic_cast<Body*>(node_); }
    else if (dynamic_cast<Script*>(node_))
    { script = dynamic_cast<Script*>(node_); }
    return Entity::Adopt(node_);
}

Actor* Actor::Clone(const bool with_child_)
{
    Actor* current = new Actor(*this);
    for (First(); !IsDone(); Next())
    { current->Add(Current()->Clone(true)); }

    return current;
}

Body* Actor::get_body(void)
{
    return body;
}

void Actor::update_body(void)
{
    collider.clear();
}

void Actor::set_collider(Actor* actor_)
{
    collider.push_back(actor_);
}

void Actor::eval(const Matter* parent_mtt_)
{
    // move (local coordinates)
    if (!dst_trans.is_origin())
    {
        Vec3 wld_trans(dst_trans.get_x(), dst_trans.get_z(), dst_trans.get_y());
        Mtx44 n_mtx;
        n_mtx.pos(wld_trans);
//         n_mtx.rotate(90, 0, 0);
        set_keyframe(KF_PROP_COORDINATE, false, 1, n_mtx.get_pos());
    }

    if (NULL != body) { update_body(); }
    if (NULL != script)
    {
        if (kf[kf_cmd].get_interval() == 0)
        {
            script->NextKf();
            if (script->IsDoneKf()) { script->FirstKf(); }
            kf_cmd = script->CurrentKf().get_cmd();
            set_keyframe(script->CurrentKf());
//             RE_DBG("script:%d, %d", kf_cmd, kf[kf_cmd].get_interval());
        }
    }

    // attack
    Entity::eval(parent_mtt_);
}

Props* Actor::gen_props(void)
{
    RE_PROPS_START(Actor);
    RE_PROP_NODE(Actor, model);
    RE_PROP_STR(Actor, forward);
    RE_PROP_STR(Actor, back);
    RE_PROP_STR(Actor, right);
    RE_PROP_STR(Actor, left);
    RE_PROP_STR(Actor, up);
    RE_PROP_STR(Actor, down);
    RE_PROP_STR(Actor, attack);
    RE_PROP_STR(Actor, all);

    RE_PROP_STR(Actor, pos);
    RE_PROP_STR(Actor, quat);
    RE_PROP_STR(Actor, color);

    return RE_PROPS_FINISH;
}

const Str& Actor::prop_model(Node* arg_)
{
    if (dynamic_cast<Model*>(arg_))
    {
        mtt->set_model(dynamic_cast<Model*>(arg_));
        RE_DBG("model: %s", arg_->get_name().c_str());
        return RE_SUCCESS;
    }
    return RE_FAILURE;
}

const Str& Actor::prop_forward(const Str& arg_)
{
    RE_DBG("forward");
    if (arg_ == PROP_TOGGLE_ON) { dst_trans.set_y(-step); }
    else { dst_trans.set_y(0); }
    return RE_SUCCESS;
}

const Str& Actor::prop_back(const Str& arg_)
{
    RE_DBG("back");
    if (arg_ == PROP_TOGGLE_ON) { dst_trans.set_y(step); }
    else { dst_trans.set_y(0); }
    return RE_SUCCESS;
}

const Str& Actor::prop_right(const Str& arg_)
{
    RE_DBG("right");
    if (arg_ == PROP_TOGGLE_ON) { dst_trans.set_x(step); }
    else { dst_trans.set_x(0); }
    return RE_SUCCESS;
}

const Str& Actor::prop_left(const Str& arg_)
{
    RE_DBG("left");
    if (arg_ == PROP_TOGGLE_ON) { dst_trans.set_x(-step); }
    else { dst_trans.set_x(0); }
    return RE_SUCCESS;
}

const Str& Actor::prop_up(const Str& arg_)
{
    RE_DBG("up");
    if (arg_ == PROP_TOGGLE_ON) { dst_trans.set_z(step); }
    else { dst_trans.set_z(0); }
    return RE_SUCCESS;
}

const Str& Actor::prop_down(const Str& arg_)
{
    RE_DBG("down");
    if (arg_ == PROP_TOGGLE_ON) {dst_trans.set_z(-step); }
    else { dst_trans.set_z(0); }
    return RE_SUCCESS;
}

const Str& Actor::prop_attack(const Str& arg_)
{
    attack_toggle = true;
    mtt->get_c_mtx().debug();

    return RE_SUCCESS;
}

const Str& Actor::prop_all(const Str& arg_)
{
    if (arg_ == PROP_TOGGLE_ON) { /* nothing */ }
    else
    {
        dst_trans.set(0, 0, 0);
        attack_toggle = false;
    }
    return RE_SUCCESS;
}

const Str& Actor::prop_pos(const Str& arg_)
{
    if (arg_.empty())
    {
//         if (mtt != NULL) { return mtt->get_c_mtx().get_pos().get_str(); }
    }

    Vec3 vec(arg_);
    Entity::set_rel_pos(vec);
    return RE_SUCCESS;
}

const Str& Actor::prop_quat(const Str& arg_)
{
//  mtt->get_c_mtx().get_quat().get_x();
    return RE_SUCCESS;
}

const Str& Actor::prop_color(const Str& arg_)
{
    if (arg_.empty()) { return RE_FAILURE; }
    Vec3 vec(arg_);
    Entity::set_color(vec);

    if (dynamic_cast<vid::RenderState*>(mtt))
    {
        RE_DBG("%s has rs", name.c_str());
    }

    return RE_SUCCESS;
}
