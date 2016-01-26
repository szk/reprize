#ifndef REPRIZE_HPP_
#define REPRIZE_HPP_

#include <string>
#include <map>

namespace reprize
{
// charactor
typedef char Char;
typedef unsigned char uChar;

/// number
// single precision float
typedef float Float32;
// double precision float
typedef double Float64;
// 16bit signed int
typedef short Int16;
// 32bit signed int
typedef int Int32;
// 64bit signed int
typedef long long Int64;
// 16bit unsigned int
typedef unsigned short uInt16;
// 32bit unsigned int
typedef unsigned int uInt32;
// 32bit unsigned int
typedef unsigned long long uInt64;
// 32bit count of amount
typedef size_t Size32;

// string
typedef std::string Str;
typedef std::istringstream ISStrm;

// common node ID type
typedef uInt64 NID;

// in scene distance unit
typedef Float32 Unit;
// typedef Float64 Unit;

// 1.00 ~ 0 Ratio
typedef Float32 Ratio;

// input code for interface
typedef uInt32 InputCode;

#define UINT64_MAX 0xFFFFFFFF
#define NID_NIL 0
#define NID_SIZE UINT64_MAX

// interface of XML
typedef std::map<const Str, const Str> StrMap;

//// map::find wrapper. const_reference cant use operator[].
#define ATT(m, s) ((m).find((s)) != (m).end() ? \
                    (m).find((s))->second : Str(""))

// other
#define PI 3.14159265358979323846f
#define EPSILON 0.0001f
#define SQRT_2 1.4142135623730951f
#define SQRT_3 1.7320508075688772f
#define HSQRT_3 0.8660254037844386f

#define NULCHAR '\0'

#define DEL(i) { if ((i) != NULL) { delete (i); (i) = NULL; } }
#define DEL_ARRAY(i) { if ((i) != NULL) { delete[] (i); (i) = NULL; } }
#define RADIAN(i) (((i) * PI) / 180)
#define DEGREE(i) (((i) * 180) / PI)

#define RE_ZERO(s, n) (memset((s), 0, (n)))
#define RE_MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define RE_MIN(a, b)  (((a) > (b)) ? (b) : (a))

#define RE_ISNUM(n) (n.find_first_not_of("1234567890-") == std::string::npos)

/// for debug message
#ifdef __FUNCTION__ // MSVC
#define AT_ __FUNCTION__ ":"
#else // GCC
#include <assert.h>
#define AT_ __PRETTY_FUNCTION__ ":"
#endif
#define RE_DBG(s,...) g_log->printf(AT_#s,__VA_ARGS__)
// #define RE_DBG(...) g_log->printf(__VA_ARGS__)
#define RE_MSG(...) g_log->printf(__VA_ARGS__)

/// property definition
#define RE_PROPS_START(t)                                  \
    typedef res::PropNode<t> t##PropNode;                  \
    typedef res::PropStr<t> t##PropStr;                    \
    typedef res::PropProp<t> t##PropProp;                  \
    res::Props* pr__ = new res::Props;

#define RE_PROPS_INHERIT(t, s)                             \
    typedef res::PropNode<t> t##PropNode;                  \
    typedef res::PropStr<t> t##PropStr;                    \
    typedef res::PropProp<t> t##PropProp;                  \
    res::Props* pr__ = s##::gen_props();

#define RE_PROP_NODE(c, n)                                      \
    pr__->Add(#n, new c##PropNode(this, &##c##::##prop_##n))
#define RE_PROP_STR(c, n)                                       \
    pr__->Add(#n, new c##PropStr(this, &##c##::##prop_##n))
#define RE_PROP_PROP(c, n)                                      \
    pr__->Add(#n, new c##PropProp(this, &##c##::##prop_##n))
#define RE_PROPS_FINISH pr__

// Constant information
#define ENGINE_NAME "Reprize"
#define DIR_BASE "base"
#define NET_DEFAULT_PORT 37564

// Common
const uInt32 FULLSCREEN          = 0;
const uInt32 SCREEN_WIDTH        = 800;
const uInt32 SCREEN_HEIGHT       = 600;
const Char* const TITLE          = "Reprize";
const Char* const VERSION        = "0.0";
const Str RE_SUCCESS           = "@RE_SUCCESS";
const Str RE_FAILURE           = "@RE_FAILURE";

const Unit Z_FAR                 = 2048;
const Unit Z_NEAR                = 1;

const Str CONFIG_FILE            = "config.xml";

// Console
const uInt32 MAX_CONSOLE_CHAR    = 512;
const uInt32 MAX_CONSOLE_HISTORY = 128;
const Str CL_SEPARATOR          = " ";

// Node name
const uInt32 MAX_NAME_LENGTH     = 128;
const uInt64 MAX_PATH_SIZE       = 512;

// VFS
const uInt32 MAX_EMPTY_NODE      = 1024;
const Str VFS_NONAME             = "";
const Str VFS_NODE_SEPARATOR     = "/";
const Str VFS_PROP_SEPARATOR     = ".";
const Str VFS_ROOT               = VFS_NODE_SEPARATOR;

// name means the specified node's name
const Str PROP_NAME              = "name";
// ref means actual reference of each node
const Str PROP_REFERENCE         = "ref";
// new means actual reference of each node
const Str PROP_NEW               = "new";
// def means the root node of definition
const Str PROP_DEFINITION        = "def";
// properties value
const Str PROP_TOGGLE_ON         = "on";
const Str PROP_TOGGLE_OFF        = "off";
const Str RE_CHECKED             = "X";

// for interface
const Str STR_ELLIPSIS    = "...";

namespace res { class Node; }

namespace tmr
{
class Timer
{
protected:
    Timer(void);
public:
    virtual ~Timer(void);
    static Timer* Instance(void);

    res::Node* get_node(void);
    virtual const bool init(void);

    virtual void reset(void);
    virtual const bool update(void);

    virtual Int32 get_passed_usec(void);
    virtual uInt32 get_epoc_sec(void);

    virtual void sleep_sec(const uInt32 s_);
    virtual void sleep_msec(const uInt32 ms_);
    virtual void sleep_usec(const uInt32 us_);
private:
    res::Node* node;
};
}

namespace inf { class Interface; class HUD; }

namespace net
{
class Network
{
public:
    Network(void);
    virtual ~Network(void);

    res::Node* get_node(void);
    const bool init(void);
    void set_view(inf::Interface* inf_);

    class Connection* add_passive(const Char* hostname, const Char* port,
                                  const Char* protocol);
    Connection* add_active(const Char* hostname_, const Char* port_,
                           const Char* protocol_);
    const bool close_socket(void);

    uInt32 get_output(void);
    uInt32 get_input(void);
    uInt32 http_request(const Str& url_, const Str& user_, const Str& pass_);
private:
    class net_pvt* pvt;
    res::Node* node;
};
}

namespace ssn { class Session; }
namespace phy { class Physics; }

namespace res
{
class IDRegistry;
class Reader;
class Loader;
class TexLoader;
class FontLoader;
class SndLoader;
class Paragraph;
class Matter;
class Entity;
class Scene;
class Actor;
class Body;
class Geometry;
class Model;
class Sound;
class Space;
class Stage;

class Resource
{
public:
    Resource(void);
    virtual ~Resource(void);

    res::Node* get_node(void);
    const bool init(void);

    void add_reader(res::Reader* reader_);
    void add_loader(res::Loader* loader_);

    const bool load(const Str& location_);
    res::Node* make_define(void);
    const bool add_define(res::Node* define_);

    class VFS* get_vfs(void) const;
    res::Loader* get_geom_loader(void) const;

    void set_physics(phy::Physics* phys_);

    void add_node(Node* node_);
    void reserve_sysres(void);
    void apply_default_config(void);

private:
    class res_pvt* pvt;
    res::Node* node;
};
}

namespace inp
{
class InputBuffer;

class Input
{
public:
    Input(void);
    virtual ~Input(void);

    res::Node* get_node(void);
    const bool init(void);
    void set_view(inf::Interface* inf_);

    void set_base(class InputBase* base_);
    void set_keyboard(class Keyboard* keyboard_);
    void set_mouse(class Mouse* mouse_);
    void set_opt_io(class OptIO* opt_io_);

    void set_ptr_relative(const bool relative_);
    const bool set_ptr(const uInt32 scr_x_, const uInt32 scr_y_);

    void restore(void);
    void check(void);
    void reset(void);

    const Str& code_to_name(const InputCode code_);
    const InputCode str_to_code(const Str& key_);

    InputBuffer& get_pressed(void);
    InputBuffer& get_released(void);

    Int32 get_ptr_x(void);
    Int32 get_ptr_y(void);
    uInt32 get_mod(void);
    const Char* get_str(void);

private:
    const Str& prop_code_to_name(const Str& arg_);
    class inp_pvt* pvt;
    res::Node* node;
};
}


namespace aud
{
class Audio
{
public:
    Audio(void);
    virtual ~Audio(void);

    res::Node* get_node(void);
    const bool init(void);
    void set_view(inf::Interface* inf_);

    void set_acoustic(class Acoustic* acoustic_);
    void set_snd_loader(res::SndLoader* snd_loader_);

    res::Loader* get_snd_loader(void);

    const bool queue(const res::Sound* snd_);
    void finish(void);

    void set_frame_d(Ratio d_);
    const bool play(void);
    const bool is_played(void);

private:
    class aud_pvt* pvt;
    res::Node* node;
};
}

namespace vid { class RenderState; }

namespace ssn
{
class Session
{
public:
    Session(void);
    virtual ~Session(void);

    res::Node* get_node(void);

    const bool init(res::VFS* vfs_);
    const bool start(res::Stage* stage_, inf::Interface* ui_);
    void add_test_circle(void);
    void set_audio(aud::Audio* audio_);

    res::Entity* get_world(void);

    void eval(void);
    void add_wl(res::Entity* ent_);
    void add_bd(res::Entity* ent_);
    void add_ol(res::Entity* ent_);
    vid::RenderState* get_wl_rs(void);
    vid::RenderState* get_bd_rs(void);
    vid::RenderState* get_ol_rs(void);

private:
    class ssn_pvt* pvt;
    res::Node* node;
};
}

namespace inf
{
class Interface
{
public:
    Interface(void);
    virtual ~Interface(void);

    res::Node* get_node(void);
    const bool init(inp::Input* input_, res::VFS* vfs_);
    const bool set_desktop(class Desktop* desktop_);
    void set_carrier(res::Actor* act_);
    void update_input(void);
    void ptr(const Int32 x_, const Int32 y_);
    const bool key(inp::InputBuffer& pressed_,
                   inp::InputBuffer& released_, const Char* str_);
    void proc(void);
    void set_frame_d(Ratio d_);

    void add_beacon(const res::Entity* mtt_);

private:
    const Str& prop_desktop(const Str& arg_);
    const Str& prop_actor(const Str& arg_);

    class inf_pvt* pvt;
    res::Node* node;
};
}

namespace phy
{
class Physics
{
public:
    Physics(void);
    virtual ~Physics(void);

    res::Node* get_node(void);
    const bool init(void);
    void set_view(inf::Interface* inf_);

    void set_law(class Collision* collision_, class Impulse* impulse_);
    void tick(void);
    void add_actor(res::Actor* actor_, const bool base_);

private:
    class phy_pvt* pvt;
    res::Node* node;
};
}

namespace utl
{
class Util
{
public:
    Util(void);
    virtual ~Util(void);

    res::Node* get_node(void);

    const bool init(void);
    void rnd_init(uInt32 seed_);
    uInt32 rnd_ui32(void);
    Int32 rnd_i32(void);
    Float32 rnd_f32(void);
    Float64 rnd_f64(void);

private:
    class utl_pvt* pvt;
    res::Node* node;
};
}

namespace vid
{
class Video
{
public:
    Video(void);
    virtual ~Video(void);

    res::Node* get_node(void);
    const bool init(void);

    void set_screen(class Screen* screen_);
    void set_renderer(class Renderer* renderer_);
    void set_geom_loader(res::Loader* geom_loader_);
    void set_tex_loader(res::TexLoader* tex_loader_);
    void set_font_loader(res::FontLoader* tex_loader_);

    res::Loader* get_tex_loader(void);
    res::Loader* get_font_loader(void);

    void begin(void);
    void finish(void);
    const bool appear(void);

    void set_frame_d(Ratio d_);
    const bool render(vid::RenderState* rs_);

    const bool flip(void);
    const bool is_drawn(void);

    Unit dev(const Str& arg_);

private:
    class vid_pvt* pvt;
    res::Node* node;
};
}

namespace mem
{
class MemHunk
{
public:
    MemHunk(void);
    virtual ~MemHunk(void);

    const bool calloc(uInt32 size_mb_);
    const bool clear(void);

    void* get_umem(Size32 size_);
    void* get_lmem(Size32 size_);

private:
    class mem_pvt* pvt;
};
}

#ifdef WIN32

// XXX for VCToolkit
#define snprintf _snprintf_s
#define vsnprintf _vsnprintf_s
#define copysign _copysign

class Win32Device
{
public:
    Win32Device(int argc, char* argv[]);
    virtual ~Win32Device(void);
    virtual vid::Video* create_video(void);
    virtual inp::Input* create_input(void);
    virtual tmr::Timer* create_timer(void);
    virtual aud::Audio* create_audio(void);
    virtual net::Network* create_network(void);
    virtual res::Resource* create_resource(void);
private:
    class win32_pvt* pvt;
};

#else
#ifdef __APPLE__

class NSDevice
{
public:
    NSDevice(int argc, char** argv);
    virtual ~NSDevice(void);
    virtual vid::Video* create_video(void);
    virtual inp::Input* create_input(void);
    void set_pos(Float32 x_, Float32 y_);
    void set_click(const bool clicked_);
    void set_scale(const Float32 scale_);

private:
    class ns_pvt* pvt;
};

#else // __APPLE__

#include <dlfcn.h>

class X11Device
{
public:
    X11Device(int argc, char* argv[]);
    virtual ~X11Device(void);
    virtual vid::Video* create_video(void);
    virtual inp::Input* create_input(void);
private:
    class x11_pvt* pvt;
};

#endif // __APPLE__

class UNIXDevice
{
public:
    UNIXDevice(int argc_, char* argv_[]);
    virtual ~UNIXDevice(void);
    virtual tmr::Timer* create_timer(void);
    virtual aud::Audio* create_audio(void);
    virtual net::Network* create_network(void);
    virtual res::Resource* create_resource(void);
private:
    class unix_pvt* pvt;
};

#endif // Win32

namespace srv
{
class Server
{
public:
    Server(void);
    virtual ~Server(void);

private:
    class srv_pvt* pvt;
    res::Node* node;
};
}

enum TextureFiltering
{
    BILINEAR,
    TRILINEAR,
    ANISOTROPIC, // XXX NOT IMPLEMENTED
};

enum LoopState { CORE_DRAW, CORE_EVAL, CORE_SLEEP , CORE_STATENUM };

class Core
{
public:
    Core(void);
    ~Core(void);

    const bool cl_init(vid::Video* video_, aud::Audio* audio_,
                       inp::Input* input_, net::Network* network_,
                       res::Resource* resource_, tmr::Timer* timer_);
    const bool cl_load(const Char* arc_dir_);
    const bool cl_start(void);
    const bool cl_loop(void);

    const bool srv_init(net::Network* network_,
                       res::Resource* resource_, tmr::Timer* timer_);
    const bool srv_load(const Char* arc_dir_);
    const bool srv_start(uInt32 port_);
    const bool srv_loop(void);

    const LoopState get_next_frame(uInt32 now_time_);
    void set_max_fps(uInt32 internal_, uInt32 visual_);

private:
    class core_pvt* pvt;
};

extern bool g_finish;
extern bool g_focus;
extern res::IDRegistry* id_reg;
extern res::Paragraph* g_log;
extern utl::Util* g_utl;

}

#endif
