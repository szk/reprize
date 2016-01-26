#include "Common.hpp"

#include "VFS.hpp"
#include "Body.hpp"
#include "scene/Cell.hpp"
#include "Script.hpp"
#include "Actor.hpp"

using namespace reprize;
using namespace std;
using namespace phy;
using namespace res;

Actor::Actor(Str name_, Model* mdl_, Body* body_, Script* script_)
    : Entity(name_, new Matter(mdl_)), body(body_), here(NULL), script(NULL),
      step(1.0), attached(false),
      forward_toggle(false), back_toggle(false), right_toggle(false),
      left_toggle(false), up_toggle(false), down_toggle(false),
      attack_toggle(false), kf_cmd(KF_CTRL_WAIT)
{
    attach();
    cmd_off_all(Str());
}

Actor::Actor(const Actor& src_)
    : Entity(src_),
      body(src_.body), here(NULL), script(src_.script),
      step(1.0), attached(false),
      forward_toggle(false), back_toggle(false), right_toggle(false),
      left_toggle(false), up_toggle(false), down_toggle(false),
      attack_toggle(false), kf_cmd(KF_CTRL_WAIT)
{
    attach();
    cmd_off_all(Str());
}

Actor::~Actor(void)
{
}

const bool Actor::Put(Node* node_)
{
    if (dynamic_cast<Body*>(node_)) { body = dynamic_cast<Body*>(node_); }
    else if (dynamic_cast<Script*>(node_))
    { script = dynamic_cast<Script*>(node_); }
    return Entity::Put(node_);
}

Actor* Actor::Clone(void) const
{
    return new Actor(*this);
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
            g_log->printf("script:%d, %d", kf_cmd, kf[kf_cmd].get_interval());
        }
    }

    // attack
    Entity::eval(parent_mtt_);
}

typedef Command<Actor> ActCmd;

void Actor::attach(void)
{
    if (attached) { return; }
    Put(new Node("+forward", new ActCmd(this, &Actor::cmd_on_forward)));
    Put(new Node("+back",    new ActCmd(this, &Actor::cmd_on_back)));
    Put(new Node("+right",   new ActCmd(this, &Actor::cmd_on_right)));
    Put(new Node("+left",    new ActCmd(this, &Actor::cmd_on_left)));
    Put(new Node("+up",      new ActCmd(this, &Actor::cmd_on_up)));
    Put(new Node("+down",    new ActCmd(this, &Actor::cmd_on_down)));
    Put(new Node("+attack",  new ActCmd(this, &Actor::cmd_on_attack)));
    Put(new Node("+all",     new ActCmd(this, &Actor::cmd_on_all)));

    Put(new Node("-forward", new ActCmd(this, &Actor::cmd_off_forward)));
    Put(new Node("-back",    new ActCmd(this, &Actor::cmd_off_back)));
    Put(new Node("-right",   new ActCmd(this, &Actor::cmd_off_right)));
    Put(new Node("-left",    new ActCmd(this, &Actor::cmd_off_left)));
    Put(new Node("-up",      new ActCmd(this, &Actor::cmd_off_up)));
    Put(new Node("-down",    new ActCmd(this, &Actor::cmd_off_down)));
    Put(new Node("-attack",  new ActCmd(this, &Actor::cmd_off_attack)));
    Put(new Node("-all",     new ActCmd(this, &Actor::cmd_off_all)));

    Put(new Node("pos",      new ActCmd(this, &Actor::cmd_pos)));
    Put(new Node("quat",     new ActCmd(this, &Actor::cmd_quat)));
    Put(new Node("color",    new ActCmd(this, &Actor::cmd_color)));

    attached = true;
}

void Actor::detach(void)
{
    if (!attached) { return; }
    attached = false;
}

Unit Actor::cmd_on_forward(const Str& arg_)
{
    forward_toggle = true;
    dst_trans.set_y(-step);
    return 0;
}

Unit Actor::cmd_on_back(const Str& arg_)
{
    back_toggle = true;
    dst_trans.set_y(step);
    return 0;
}

Unit Actor::cmd_on_right(const Str& arg_)
{
    right_toggle = true;
    dst_trans.set_x(step);
    return 0;
}

Unit Actor::cmd_on_left(const Str& arg_)
{
    left_toggle = true;
    dst_trans.set_x(-step);
    return 0;
}

Unit Actor::cmd_on_up(const Str& arg_)
{
    up_toggle = true;
    dst_trans.set_z(step);

    return 0;
}

Unit Actor::cmd_on_down(const Str& arg_)
{
    down_toggle = true;
    dst_trans.set_z(-step);
    return 0;
}

Unit Actor::cmd_on_attack(const Str& arg_)
{
    attack_toggle = true;
    mtt->get_c_mtx().debug();

    return 0;
}

Unit Actor::cmd_on_all(const Str& arg_)
{
    forward_toggle = back_toggle = right_toggle = left_toggle =
        up_toggle = down_toggle = attack_toggle = true;
    dst_trans.set(0, 0, 0);
    return 0;
}

Unit Actor::cmd_off_forward(const Str& arg_)
{
    forward_toggle = false;
    if (back_toggle) { cmd_on_back(def_arg); }
    else { dst_trans.set_y(0); }
    return 0;
}

Unit Actor::cmd_off_back(const Str& arg_)
{
    back_toggle = false;
    if (forward_toggle) { cmd_on_forward(def_arg); }
    else { dst_trans.set_y(0); }
    return 0;
}

Unit Actor::cmd_off_right(const Str& arg_)
{
    right_toggle = false;
    if (left_toggle) { cmd_on_left(def_arg); }
    else { dst_trans.set_x(0); }
    return 0;
}

Unit Actor::cmd_off_left(const Str& arg_)
{
    left_toggle = false;
    if (right_toggle) { cmd_on_right(def_arg); }
    else { dst_trans.set_x(0); }
    return 0;
}

Unit Actor::cmd_off_up(const Str& arg_)
{
    up_toggle = false;
    if (down_toggle) { cmd_on_down(def_arg); }
    else { dst_trans.set_z(0); }
    return 0;
}

Unit Actor::cmd_off_down(const Str& arg_)
{
    down_toggle = false;
    if (up_toggle) { cmd_on_up(def_arg); }
    else { dst_trans.set_z(0); }
    return 0;
}

Unit Actor::cmd_off_attack(const Str& arg_)
{
    attack_toggle = false;
    return 0;
}

Unit Actor::cmd_off_all(const Str& arg_)
{
    forward_toggle = back_toggle = right_toggle = left_toggle =
        up_toggle = down_toggle = attack_toggle = false;
    dst_trans.set(0, 0, 0);
    return 0;
}

Unit Actor::cmd_pos(const Str& arg_)
{
    if (arg_.empty()) { return mtt->get_c_mtx().get_pos().get_x(); }
    Vec3 vec(arg_);
    Entity::set_rel_pos(vec);
    return 1;
}

Unit Actor::cmd_quat(const Str& arg_)
{
    return mtt->get_c_mtx().get_quat().get_x();
}

Unit Actor::cmd_color(const Str& arg_)
{
    if (arg_.empty()) { return mtt->get_c_mtx().get_pos().get_x(); }
    Vec3 vec(arg_);
    Entity::set_color(vec);

    cerr << this << endl;

    return 1;
}
