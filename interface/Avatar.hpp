#ifndef AVATAR_HPP_
#define AVATAR_HPP_

#include "input/InputBase.hpp"
#include "input/InputBuffer.hpp"
#include "resource/Entity.hpp"

namespace reprize
{
namespace res { class VFS; class Node; class Entity; }

namespace inf
{
class Avatar : public res::Entity
{
public:
    Avatar(void);
    virtual ~Avatar(void);

    const bool init(inp::Input* input_, res::VFS* vfs_);

    void eval(const res::Matter* parent_mtt_);
    void enable(void);
    void disable(void);

    void set_carrier(res::Actor* carrier_);

    const bool key(inp::InputBuffer& pressed_, inp::InputBuffer& released_);
    const bool input_ptr(const Int32 x_, const Int32 y_, const Ratio frame_d_);
    res::Entity* pick_session(const Int32 x_, const Int32 y_);
    const bool str(const Char* str_);
    const class HUD* get_hud(void) const;

protected:
    const Str& prop_quit(const Str& arg_);
    const Str& prop_beacon(const Str& arg_);
    const Str& prop_sensitivity(const Str& arg_);
    const Str& prop_carrier(const Str& arg_);

    const Str& prop_bind(const Str& arg_);
    const Str& prop_ptr(const Str& arg_);

    const Str& prop_personview(const Str& arg_); // or tps

    res::Entity* carrier;
    res::Props* carrier_props;
    class Control* ctrl;
    class Pointer* pntr;

    inp::Input* input;
    HUD* hud;
    class Manipulator* manipulator;
    class Picker* picker;

    Mtx44 victim_mtx;
    Vec3 ptr_pos;
    Quat lookat_quat;
    Str def_arg;
    Unit sensitivity;
};
}
}

#endif
