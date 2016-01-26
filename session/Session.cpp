#include "Common.hpp"

#include "video/RenderState.hpp"

#include "resource/VFS.hpp"
#include "resource/Actor.hpp"
#include "resource/Text.hpp"

#include "resource/scene/Scene.hpp"
#include "resource/scene/Space.hpp"
#include "resource/scene/Metric.hpp"
#include "resource/scene/Topology.hpp"
#include "resource/Stage.hpp"

#include "interface/Desktop.hpp"


using namespace reprize;
using namespace vid;
using namespace ssn;
using namespace res;
using namespace inf;
using namespace std;

Session::Session(void)
    : node(NULL)
{
    pvt = new ssn_pvt;
    node = new Node("Session");
}

Session::~Session(void)
{
    DEL(pvt);
    DEL(node);
}

res::Node* Session::get_node(void)
{
    return node;
}

const bool Session::init(VFS* vfs_)
{
    pvt->vfs = vfs_;
    pvt->mtt = new Matter;
    return true;
}

const bool Session::start(Stage* prop_, Interface* ui_)
{
    Stage* stage = dynamic_cast<Stage*>
        (pvt->vfs->find("/System/Stage/SysStage"));

    if (!stage || !stage->get_overlay() || !stage->get_world()
        || !stage->get_backdrop() || !stage->get_menu() || !stage->get_hud())
    {
        RE_DBG("scene is not enough");
        stage = dynamic_cast<Stage*>(pvt->vfs->find("/System/Stage/SysStage"));
    }

    // Set each scene / menu
    pvt->overlay = stage->get_overlay();
    pvt->world = stage->get_world();
    pvt->backdrop = stage->get_backdrop();
    pvt->menu = stage->get_menu();
    pvt->hud = stage->get_hud();

    // Set each renderstate for rendering
    pvt->overlay_rs = pvt->overlay->get_rs();
    node->Add(pvt->overlay);
    pvt->world_rs = pvt->world->get_rs();
    node->Add(pvt->world);
    pvt->backdrop_rs = pvt->backdrop->get_rs();
    node->Add(pvt->backdrop);

    // Set each pov
    if (!pvt->overlay->get_pov())
    {
        Actor* ol_pov = new Actor("ol_POV", new Model("ol_pov_mdl"));
        pvt->overlay->Add(ol_pov);
        pvt->overlay->set_pov(ol_pov);
        ol_pov->rotate(Vec3(180, 0, 0));
        ol_pov->translate(Vec3(0, -1 * static_cast<Int32>(SCREEN_HEIGHT), 32));
    }

    if (!pvt->world->get_pov())
    {
        Actor* wl_pov = new Actor("wl_POV", new Model("wl_pov_mdl"));
        pvt->world->Add(wl_pov);
        pvt->world->set_pov(wl_pov);
        wl_pov->set_rel_pos(Vec3(0, 0, 0));
    }

    if (!pvt->backdrop->get_pov())
    {
        Actor* bd_pov = new Actor("bd_POV", new Model("bd_pov_mdl"));
        pvt->backdrop->Add(bd_pov);
        pvt->backdrop->set_pov(bd_pov);
        bd_pov->set_rel_pos(Vec3(0, 0, 0));
    }

    pvt->overlay->Add(pvt->hud);
    pvt->overlay->Add(pvt->menu);

    ui_->set_carrier(pvt->world->get_pov());
    ui_->set_desktop(pvt->menu);

    // first evaluation
    eval();
    add_test_circle();

    return true;
}

void Session::add_test_circle(void)
{
    Geometry* geom = new Geometry("test");
    geom->add_mesh(res::LINE_LOOP);
    uInt32 lv = 16;
    for (uInt32 i = 0; lv > i; ++i)
    {
        Unit x = sinf(static_cast<Unit>((PI * 2) * i / lv));
        Unit z = cosf(static_cast<Unit>((PI * 2) * i / lv));
        geom->add_mesh_idx(i);
        geom->add_vertex(x * 1, z * 1, -10.0, 0, 0, 1, 0, 0, 1);
    }
    Model* mdl = dynamic_cast<Model*>(pvt->vfs->find("/System/Model/Plane_mdl"));
    Actor* circle = new res::Actor("TestCircle",// mdl);
                                   new Model("testcircle_mdl", geom));
    pvt->world->Add(circle);
}

void Session::set_audio(aud::Audio* audio_)
{
    pvt->audio = audio_;
}

res::Entity* Session::get_world(void)
{
    return pvt->world;
}

void Session::eval(void)
{
    if (pvt->world) { pvt->world->eval(pvt->mtt); }
    if (pvt->backdrop) { pvt->backdrop->eval(pvt->mtt); }
    if (pvt->overlay) { pvt->overlay->eval(pvt->mtt); }
    g_log->refresh();
}

RenderState* Session::get_wl_rs(void)
{
    pvt->world_rs->set_pov(pvt->world->get_pov()->get_mtt()->get_c_mtx());
    return pvt->world_rs;
}

RenderState* Session::get_bd_rs(void)
{
    // sync the quat with the world pov
    Mtx44 mtx(pvt->world->get_pov()->get_mtt()->get_c_mtx().get_quat());
    pvt->backdrop_rs->set_pov(mtx);
    return pvt->backdrop_rs;
}

RenderState* Session::get_ol_rs(void)
{
    pvt->overlay_rs->set_pov(pvt->overlay->get_pov()->get_mtt()->get_c_mtx());
    return pvt->overlay_rs;
}

void Session::add_wl(res::Entity* ent_)
{
    pvt->world->Add(ent_);
}

void Session::add_bd(res::Entity* ent_)
{
    pvt->backdrop->Add(ent_);
}

void Session::add_ol(res::Entity* ent_)
{
    pvt->overlay->Add(ent_);
}
