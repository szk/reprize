#include "Common.hpp"

#include "resource/Texture.hpp"
#include "resource/Font.hpp"

#include "Avatar.hpp"
#include "HUD.hpp"

#include "resource/Actor.hpp"
#include "Desktop.hpp"

using namespace std;
using namespace reprize;
using namespace res;
using namespace inf;

Interface::Interface(void)
    : node(NULL)
{
    pvt = new inf_pvt;
    node = new Node("Interface");
    pvt->ui_toggled = true;
}

Interface::~Interface(void)
{}

Node* Interface::get_node(void)
{
    return node;
}

const bool Interface::init(inp::Input* input_, VFS* vfs_)
{
    pvt->input = input_;
    pvt->vfs = vfs_;

    pvt->avatar = new Avatar;
    node->Add(pvt->avatar);
    pvt->avatar->init(input_, vfs_);
//     session_->add_ol(pvt->avatar->get_hud()->get_ol());
#ifdef UNDEF
    node->Add(new Node("desktop", new InfPropStr(this, &Interface::prop_desktop)));
    node->Add(new Node("actor", new InfPropStr(this, &Interface::prop_actor)));
#endif
    return true;
}

const bool Interface::set_desktop(Desktop* desktop_)
{
    if (desktop_)
    {
        pvt->desktop = desktop_;
//         node->Add(pvt->desktop);
//         pvt->session->add_ol(pvt->desktop);
        return true;
    }
    return false;
}

void Interface::set_carrier(res::Actor* act_)
{
    if (!act_) { return; }
    pvt->avatar->set_carrier(act_);
}

void Interface::ptr(const Int32 x_, const Int32 y_)
{
    if (!pvt->desktop) { return; }
    if (pvt->ui_toggled)
    {
        if (pvt->desktop->pick_widget(x_, y_)) {}
        else
        {
            Entity* picked = pvt->avatar->pick_session(x_, y_);
            // get an entity from the session
            pvt->desktop->pick_entity(x_, y_, picked);
        }
    }
    else { pvt->avatar->input_ptr(x_, y_, pvt->frame_d); }
}

void Interface::update_input(void)
{
    pvt->input->check();
    ptr(pvt->input->get_ptr_x(), pvt->input->get_ptr_y());
}

void Interface::proc(void)
{
    key(pvt->input->get_pressed(), pvt->input->get_released(),
        pvt->input->get_str());

    pvt->input->reset();
}

void Interface::set_frame_d(Ratio d_)
{
    pvt->frame_d = d_;
}

void Interface::add_beacon(const Entity* ent_ = NULL)
{
    return;
    if (ent_ == NULL) { RE_DBG("Beacon is null."); }
//     Geometry* geom = pvt->prim->build_circle("beacon_circle", 100, 20);

//     Entity* beacon = new Entity("beacon_entity",
//                                 new Model("circle", geom, NULL));

//     beacon->set_rel_pos(Vec3(0, 100, 100));

//     pvt->hud->Add(beacon);

//     pvt->hud->add_beacon(B_PIE, Vec3(0, 0, 0), 0, pvt->picker);

//     if (pvt->vfs->find("/Session/Subject") != NULL)
//     {
//         pvt->hud->add_beacon(B_CAPTION, Vec3(0, 20, 0), 0,
//                              pvt->vfs->find("/Session/Subject"));
//     }

//     pvt->hud->add_beacon(B_CAPTION, Vec3(0, 30, 0), 0, pvt->picker);

}

const bool Interface::key(inp::InputBuffer& pressed_,
                          inp::InputBuffer& released_, const Char* str_)
{
    if (!pvt->desktop) { return false; }

    for (pressed_.First(); !pressed_.IsDone(); pressed_.Next())
    {
        if (pressed_.Current() == K_ESCAPE)
        {
            if (pvt->ui_toggled)
            {
                pvt->ui_toggled = false;
                pvt->desktop->hide();
                pvt->avatar->enable();
            }
            else
            {
                pvt->ui_toggled = true;
                pvt->avatar->disable();
                pvt->desktop->show();
                Vec3 ptr = pvt->desktop->get_ptr_pos();
//                 // revert from relative pointer
                pvt->input->set_ptr(static_cast<uInt32>(ptr.get_x()),
                                    static_cast<uInt32>(ptr.get_y()));
//                 pvt->desktop->input_ptr(ptr, ptr, false, false);
            }

            pvt->input->set_ptr_relative(!pvt->ui_toggled);
            pvt->input->reset();
            return false;
        }
    }

    if (pvt->ui_toggled)
    {
        pvt->desktop->key(pressed_, released_);
        pvt->desktop->str(str_);
    }
    else
    {
        pvt->avatar->key(pressed_, released_);
        pvt->avatar->str(str_);
    }
    return true;
}

const Str& Interface::prop_desktop(const Str& arg_)
{
    if (arg_.empty()) { return RE_FAILURE; }
    Desktop* tgt = dynamic_cast<Desktop*>(pvt->vfs->find(arg_));
    if (tgt) { set_desktop(tgt); return RE_SUCCESS; }

    return RE_SUCCESS;
}

const Str& Interface::prop_actor(const Str& arg_)
{
    if (arg_.empty()) { return RE_FAILURE; }
    Desktop* tgt = dynamic_cast<Desktop*>(pvt->vfs->find(arg_));
    if (tgt) { set_desktop(tgt); return RE_SUCCESS; }

    return RE_SUCCESS;
}
