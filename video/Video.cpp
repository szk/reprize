#include "Common.hpp"

#include "Screen.hpp"

#include "Renderer.hpp"
#include "resource/loader/GeomLoader.hpp"
#include "resource/loader/TexLoader.hpp"
#include "resource/loader/FontLoader.hpp"

using namespace reprize;
using namespace vid;

Video::Video(void)
    : node(NULL)
{
    pvt = new vid_pvt;
    node = new res::Node("Video", NULL, true);
}

Video::~Video(void)
{
    DEL(pvt->screen);
    DEL(pvt->renderer);
    DEL(pvt);
    DEL(node);
}

res::Node* Video::get_node(void)
{
    return node;
}

void Video::set_screen(Screen* screen_)
{
    pvt->screen = screen_;
}

void Video::set_renderer(Renderer* renderer_)
{
    pvt->renderer = renderer_;
}

void Video::set_geom_loader(res::Loader* geom_loader_)
{
    if (dynamic_cast<res::GeomLoader*>(geom_loader_))
    { pvt->geom_loader = dynamic_cast<res::GeomLoader*>(geom_loader_); }
}

void Video::set_tex_loader(res::TexLoader* tex_loader_)
{
    pvt->tex_loader = tex_loader_;
}

void Video::set_font_loader(res::FontLoader* font_loader_)
{
    pvt->font_loader = font_loader_;
}

res::Loader* Video::get_tex_loader(void)
{
    return pvt->tex_loader;
}

res::Loader* Video::get_font_loader(void)
{
    return pvt->font_loader;
}

const bool Video::init(void)
{
    node->Add(pvt->renderer->get_node());
    RE_DBG("Initializing");
    pvt->screen->init();
    RE_DBG("Screen initialized");
    pvt->renderer->init(SCREEN_WIDTH, SCREEN_HEIGHT,
                        pvt->geom_loader->get_def_geometry(),
                        pvt->tex_loader->get_def_texture());
    RE_DBG("Renderer initialized");
    pvt->font_loader->set_tex_loader(pvt->tex_loader);
    RE_DBG("Default texture resources initialized");
    RE_DBG("Initialized");

    return true;
}

void Video::begin(void)
{
    pvt->screen->begin_paint();
}

void Video::finish(void)
{
    pvt->screen->finish_paint();
}

const bool Video::appear(void)
{
    pvt->screen->appear();
    return true;
}

void Video::set_frame_d(Ratio d_)
{
    pvt->frame_d = d_;
}

const bool Video::render(vid::RenderState* rs_)
{
    pvt->renderer->set_frame_d(pvt->frame_d);
    pvt->renderer->render(rs_);

    return false;
}

const bool Video::flip(void)
{
    pvt->screen->flip();
    return 0;
}

const bool Video::is_drawn(void)
{
    return 0;
}
