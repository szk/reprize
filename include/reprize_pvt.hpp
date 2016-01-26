#ifndef REPRIZE_PVT_HPP_
#define REPRIZE_PVT_HPP_

namespace reprize
{
namespace res
{
class res_pvt
{
public:
    res_pvt(void) : var_loader(NULL), scrpt_loader(NULL), geom_loader(NULL),
                    vfs(NULL), define_root(NULL), fallback_mtl(NULL),
                    fallback_fn(NULL)
        {}
    ~res_pvt(void) {}

    class VarLoader* var_loader;
    class ScrptLoader* scrpt_loader;
    class GeomLoader* geom_loader;
    VFS* vfs;
    std::list<Reader*> reader_l;
    std::list<Node*> define_ls;
    Node* define_root;
    class Material* fallback_mtl;
    class Font* fallback_fn;
};
}

namespace inp
{
class inp_pvt
{
public:
    inp_pvt(void) : base(NULL), keyboard(NULL), mouse(NULL), opt_io(NULL),
                    inf(NULL) {}
    ~inp_pvt(void) {}

    InputBase* base;
    Keyboard* keyboard;
    Mouse* mouse;
    OptIO* opt_io;

    inf::Interface* inf;
};
}

namespace net
{
class net_pvt
{
public:
};
}

namespace inf
{
class inf_pvt
{
public:
    inf_pvt(void) : input(NULL), session(NULL), vfs(NULL), desktop(NULL),
                    op_layer(NULL), avatar(NULL), frame_d(0), ui_toggled(true)
        {}
    ~inf_pvt(void) {}

    inp::Input* input;
    ssn::Session* session;
    res::VFS* vfs;
    class Desktop* desktop;
    class res::Entity* op_layer;
    class Avatar* avatar;

    Ratio frame_d;
    bool ui_toggled;
};
}

namespace phy
{
class phy_pvt
{
public:
    phy_pvt(void) : vfs(NULL), collision(NULL), impulse(NULL) {}
    ~phy_pvt(void) {}

    res::VFS* vfs;
    inf::Interface* inf;
    Collision* collision;
    Impulse* impulse;
};
}

namespace aud
{
class aud_pvt
{
public:
    aud_pvt(void) : acoustic(NULL), snd_loader(NULL), frame_d(0)
        {}
    ~aud_pvt(void) {}

    Acoustic* acoustic;
    res::SndLoader* snd_loader;

    Ratio frame_d;
};
}

namespace ssn
{
class ssn_pvt
{
public:
    ssn_pvt(void) : mtt(NULL), audio(NULL), vfs(NULL), overlay(NULL),
                    world(NULL), backdrop(NULL), menu(NULL), hud(NULL),
                    world_rs(NULL), overlay_rs(NULL), backdrop_rs(NULL) {}
    ~ssn_pvt(void) {}

    res::Matter* mtt;
    aud::Audio* audio;
    res::VFS* vfs;
    res::Scene* overlay,* world,* backdrop;
    inf::Desktop* menu;
    class inf::HUD* hud;
    vid::RenderState* world_rs,* overlay_rs,* backdrop_rs;
};
}

namespace utl
{
class utl_pvt
{
public:
    utl_pvt(void)  {}
    ~utl_pvt(void) {}
};
}

namespace vid
{
class vid_pvt
{
public:
    vid_pvt(void) : screen(NULL), renderer(NULL), geom_loader(NULL),
                    tex_loader(NULL), font_loader(NULL), frame_d(0) {}
    ~vid_pvt(void) {}

    Screen* screen;
    Renderer* renderer;
    res::GeomLoader* geom_loader;
    res::TexLoader* tex_loader;
    res::FontLoader* font_loader;

    Ratio frame_d;
};
}

namespace mem
{
class mem_pvt
{
public:
    mem_pvt(void) : hunk(NULL), hunk_size(0) {}
    ~mem_pvt(void) {}

    void* hunk;
    Size32 hunk_size;
//    std::multimap<Size32, void*> avail_mem;
};
}

#ifdef WIN32
#include "misc/Win32DepInfo.hpp"

class win32_pvt
{
public:
    win32_pvt(void) : win32_dep_info(NULL) {}
    ~win32_pvt(void) {}

    Win32DepInfo* win32_dep_info;
};

#else
#ifdef __APPLE__

class ns_pvt
{
public:
    ns_pvt(void) : ns_dep_info(NULL) {}
    ~ns_pvt(void) {}

    class NSDepInfo* ns_dep_info;
};

#else // __APPLE__

class x11_pvt
{
public:
    x11_pvt(void) : x11_dep_info(NULL) {}
    ~x11_pvt(void) {}

    class X11DepInfo* x11_dep_info;
};

#endif // __APPLE__

class unix_pvt
{
public:
    unix_pvt(void) {}
    ~unix_pvt(void) {}
};

#endif // Win32


namespace srv
{
class srv_pvt
{
public:
    srv_pvt(void) : network(NULL), resource(NULL), timer(NULL) {}
    ~srv_pvt(void) {}

    net::Network* network;
    res::Resource* resource;
    tmr::Timer* timer;
};
}

class bld_pvt
{
public:
    bld_pvt(void) : vfs(NULL), parent(NULL), tex_loader(NULL),
                    font_loader(NULL) {}
    ~bld_pvt(void) {}

    Str def_geom, def_tex, def_spc, def_snd, type_name;
    res::VFS* vfs;
    res::Node* parent;
    std::stack<res::Node*> parent_stack;
    res::GeomLoader* geom_loader;
    res::TexLoader* tex_loader;
    res::FontLoader* font_loader;
    res::SndLoader* snd_loader;
    phy::Physics* tmp_physics;
};

class core_pvt
{
public:
    core_pvt(void) : video(NULL), audio(NULL), input(NULL), network(NULL),
                     resource(NULL), timer(NULL), ui(NULL), physics(NULL),
                     evaled_frame_n(0), drawn_frame_n(0), eval_interval(0),
                     draw_interval(0), eval_remain(0), draw_remain(0),
                     sleep_time(0) {}
    ~core_pvt(void) {}

    vid::Video* video;
    aud::Audio* audio;
    inp::Input* input;
    net::Network* network;
    res::Resource* resource;
    ssn::Session* session;
    tmr::Timer* timer;
    inf::Interface* ui;
    phy::Physics* physics;

    uInt32 internal_max_fps, visual_max_fps;
    uInt64 evaled_frame_n, drawn_frame_n;

    Int32 eval_interval, draw_interval; // 1000000 / fps (usec)
    Int32 eval_remain, draw_remain, sleep_time;
};

}
#endif
