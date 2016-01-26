#include "Common.hpp"

#include "resource/Entity.hpp"
#include "physics/Ray.hpp"

#include "Control.hpp"
#include "Pointer.hpp"

#include "HUD.hpp"
#include "Picker.hpp"
#include "Avatar.hpp"

using namespace reprize;
using namespace res;
using namespace inp;
using namespace inf;
using namespace std;

Avatar::Avatar()
    : Entity("Avatar"), carrier(NULL), carrier_props(NULL), input(NULL),
      hud(NULL), manipulator(NULL), picker(NULL), sensitivity(0)
{
    mtt = new res::Matter;
}

Avatar::~Avatar(void)
{
}

const bool Avatar::init(Input* input_, VFS* vfs_)
{
    if (!input_ || !vfs_) { return false; }
    input = input_;

    hud = new HUD(vfs_);
    Add(hud);

    picker = new Picker("Picker");
// XXX
//     picker->Add(vfs_->find("/System/Model/Ico"));
    picker->Add(new phy::Ray);
    Add(picker);

//     Props* pr_ctrl = new Props;
//     pr_ctrl->Add("bind", new AvPropStr(this, &Avatar::prop_bind));
//    pr_ctrl->Add("toggle", new AvPropStr(this, &Avatar::prop_toggle));
    ctrl = new Control;
    ctrl->init(input_);
    Add(ctrl);

    pntr = new Pointer;
    pntr->init(input_);
    Add(pntr);

#ifdef PROP
    Props* pr = new Props;
    pr->Add("quit", new AvPropStr(this, &Avatar::prop_quit));
    pr->Add("beacon", new AvPropStr(this, &Avatar::prop_beacon));
    pr->Add("carrier", new AvPropStr(this, &Avatar::prop_carrier));
#endif
    return true;
}

void Avatar::eval(const Matter* parent_mtt_)
{
    Entity::eval(parent_mtt_); // eval as entity
}

void Avatar::enable(void)
{
    hud->enable();
}

void Avatar::disable(void)
{
//     res::Node* off_toggle = carrier->find("-all");
//     if (off_toggle) { off_toggle->exec(def_arg); }
}

void Avatar::set_carrier(Actor* carrier_)
{
    if (!carrier_) { RE_DBG("carrier not found"); return; }
    DEL(carrier_props);

    carrier = carrier_;
    carrier_props = carrier->gen_props();

    ctrl->set_prop(carrier_props);

    RE_DBG("carrier found: %s", carrier->get_name().c_str());
//     carrier->Add(picker);
//     carrier->Add(this);
}

const bool Avatar::key(InputBuffer& pressed_, InputBuffer& released_)
{
    ctrl->key(pressed_, released_);
    return true;
}

const bool Avatar::input_ptr(const Int32 x_, const Int32 y_,
                             const Ratio frame_d_)
{
    picker->move_to(x_, y_);
    Mtx44 fps_mtx = picker->get_fps(sensitivity);
    Mtx44 norm_mtx = fps_mtx;
/*
    norm_mtx.inverse();
    norm_mtx.rotate(-90, 90, 0);
    norm_mtx.pos(carrier->get_rel_pos());

    carrier->set_rel_mtx(norm_mtx);
*/
    norm_mtx.inverse();
    norm_mtx.rotate(-90, 90, 0);
    norm_mtx.pos(carrier->get_rel_pos());

    carrier->set_rel_mtx(norm_mtx);

//     carrier->set_dir(norm_mtx.get_quat());

//     Vec3 c_pos = carrier->get_mtt()->get_c_mtx().get_pos();
//     Vec3 n_pos = carrier->get_mtt()->get_n_mtx().get_pos();

//     Vec3 si_pos = (n_pos - c_pos) * frame_d_ + c_pos;

    return false;
}

res::Entity* Avatar::pick_session(const Int32 x_, const Int32 y_)
{
    picker->move_to(x_, y_);
    picker->screen_to_dir(Vec3(static_cast<Unit>(SCREEN_WIDTH),
                               static_cast<Unit>(SCREEN_HEIGHT)), 90);
//                           dynamic_cast<vid::RenderState*>(mtt)->get_fov());
/*
    RE_DBG("avatar dir: ";
    picker->get_rel_mtx().get_dir().debug();
*/
    return picker->get_collided();
}

const bool Avatar::str(const Char* str_)
{
    return false;
}

const HUD* Avatar::get_hud(void) const
{
    return hud;
}

const Str& Avatar::prop_quit(const Str& arg_)
{
    g_finish = true;

    RE_DBG("quit");

    return RE_SUCCESS;
}

const Str& Avatar::prop_beacon(const Str& arg_)
{
    return RE_SUCCESS;
}

const Str& Avatar::prop_carrier(const Str& arg_)
{
//     res::Entity* src = dynamic_cast<res::Entity*>(vfs->find(arg_));
//     if (src) { carrier = src; return 1; }
    return RE_SUCCESS;
}

const Str& Avatar::prop_ptr(const Str& arg_)
{
//     RE_DBG("ptr: %s", arg_.c_str());
    return RE_SUCCESS;
}

const Str& Avatar::prop_personview(const Str& arg_)
{
    return RE_SUCCESS;
}
