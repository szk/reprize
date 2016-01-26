#include "Common.hpp"

#include <iostream>
#include <fstream>

bool reprize::g_finish;
bool reprize::g_focus;
reprize::res::Paragraph* reprize::g_log;
reprize::utl::Util* reprize::g_utl;

using namespace reprize;

Core::Core(void)
    : pvt(NULL)
{
    pvt = new core_pvt;
}

Core::~Core(void)
{
}

const bool Core::cl_init(vid::Video* video_, aud::Audio* audio_,
                         inp::Input* input_, net::Network* network_,
                         res::Resource* resource_, tmr::Timer* timer_)
{
    pvt->resource = resource_;
    pvt->video = video_;
    pvt->audio = audio_;
    pvt->input = input_;
    pvt->network = network_;
    pvt->timer = timer_;

    set_max_fps(20, 60);

    // independent
    pvt->physics = new phy::Physics();
    pvt->ui = new inf::Interface();
    pvt->scene = new scn::Scene();

    g_utl = new utl::Util();
    g_utl->init();

    // resource
    pvt->resource->init();
    pvt->video->set_geom_loader(pvt->resource->get_geom_loader());

    // video
    pvt->video->init();
    pvt->video->appear();
    pvt->resource->add_node(pvt->video->get_node());

    // audio
    pvt->audio->init();
    pvt->resource->add_node(pvt->audio->get_node());
    pvt->scene->set_audio(pvt->audio);

    // add loaders
    pvt->resource->add_loader(pvt->video->get_tex_loader());
    pvt->resource->add_loader(pvt->video->get_font_loader());
    pvt->resource->add_loader(pvt->audio->get_snd_loader());
    pvt->resource->add_loader(pvt->resource->get_geom_loader());
    pvt->resource->add_loader(pvt->resource->get_scrpt_loader());
    pvt->resource->add_loader(pvt->resource->get_var_loader());

    // input
    pvt->input->init();
    pvt->input->restore();
    pvt->input->check();
    pvt->resource->add_node(pvt->input->get_node());

    // network
    pvt->resource->add_node(pvt->network->get_node());

    // timer
    pvt->timer->init();
    pvt->resource->add_node(pvt->timer->get_node());

    g_utl->rnd_init(pvt->timer->get_epoc_sec());

    // physics
    pvt->physics->init();
    pvt->resource->add_node(pvt->physics->get_node());

    // scene
    pvt->scene->init(pvt->resource->get_vfs());
    pvt->resource->add_node(pvt->scene->get_node());

    // interface
    pvt->ui->init(pvt->input, pvt->resource->get_vfs(), pvt->scene);
    pvt->resource->add_node(pvt->ui->get_node());

    // interface / beacon
    pvt->audio->set_view(pvt->ui);
    pvt->input->set_view(pvt->ui);
    pvt->network->set_view(pvt->ui);
    pvt->physics->set_view(pvt->ui);

    pvt->resource->reserve_sysres();
    return true;
}

const bool Core::cl_load(const Char* arc_dir_)
{
    pvt->timer->reset();

    // load
    pvt->resource->load(Str(arc_dir_));

    // the default environment
    pvt->scene->prepare();

    // initial evaluation
    pvt->ui->proc();
    pvt->scene->eval();

//     pvt->physics->add_actor(pvt->scene->get_world());
    pvt->resource->get_vfs()->debug();
    return true;
}

const bool Core::cl_loop(void)
{
    pvt->timer->reset();

    switch (get_next_frame(pvt->timer->get_passed_usec()))
    {
    case CORE_DRAW:
        if (pvt->draw_remain > 1000000 || pvt->eval_remain > 1000000) // FPS > 1
        { pvt->video->set_frame_d(0); pvt->ui->set_frame_d(0); }
        else
        {
            Ratio i = 1 - (static_cast<Ratio>(pvt->eval_remain)
                           / pvt->eval_interval);
            pvt->video->set_frame_d(i);
            pvt->ui->set_frame_d(i);
        }

        // usual
        pvt->ui->update_input();
        pvt->video->begin();

        pvt->video->render(pvt->scene->get_bd_rs());
        pvt->video->render(pvt->scene->get_wl_rs());
        pvt->video->render(pvt->scene->get_ol_rs());

        pvt->video->finish();
        pvt->video->flip();
        break;

    case CORE_EVAL:
        pvt->scene->eval();
        pvt->audio->play();
        pvt->ui->proc();
        pvt->physics->tick();
        break;

    case CORE_SLEEP:
        pvt->timer->sleep_usec(pvt->sleep_time);
        break;
    case CORE_STATENUM:
        break;
    }
    pvt->timer->update();

    return true;
}

const bool Core::srv_init(net::Network* network_,
                         res::Resource* resource_, tmr::Timer* timer_)
{
    return false;
}

const bool Core::srv_load(const Char* arc_dir_)
{
    return false;
}

const bool Core::srv_start(uInt32 port_)
{
    return false;
}

const LoopState Core::get_next_frame(uInt32 now_time_)
{
    // precondition is eval_interval > draw_interval.
    pvt->eval_remain -= now_time_;
    pvt->draw_remain -= now_time_;

    if (pvt->eval_remain <= 0)
    { pvt->eval_remain = pvt->eval_interval; return CORE_EVAL; }
    else if (pvt->draw_remain <= 0)
    { pvt->draw_remain = pvt->draw_interval; return CORE_DRAW; }

    if (pvt->eval_remain > pvt->draw_remain)
    { pvt->sleep_time = pvt->draw_remain; }
    else { pvt->sleep_time = pvt->eval_remain; }

    return CORE_SLEEP;
}

void Core::set_max_fps(uInt32 internal_, uInt32 visual_)
{
    if (internal_ > visual_)
    {
        pvt->internal_max_fps = 10;
        pvt->visual_max_fps = 30;
    }
    else
    {
        pvt->internal_max_fps = internal_;
        pvt->visual_max_fps = visual_;
    }

    pvt->eval_interval = 1000000 / pvt->internal_max_fps;
    pvt->draw_interval = 1000000 / pvt->visual_max_fps;
    pvt->eval_remain = pvt->eval_interval;
    pvt->draw_remain = pvt->draw_interval;
}

const bool Core::add_blueprint(Blueprint* bp_)
{
    if (bp_  == NULL || pvt->resource == NULL) { return false; }
    return pvt->resource->add_bpnode(bp_) != NULL;
}
