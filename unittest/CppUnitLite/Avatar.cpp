#include "Common.hpp"

#include "resource/Entity.hpp"
#include "physics/Ray.hpp"

#include "HUD.hpp"
#include "Picker.hpp"
#include "Avatar.hpp"

using namespace reprize;
using namespace res;
using namespace inp;
using namespace scn;
using namespace inf;
using namespace std;

Avatar::Avatar()
    : Entity("Avatar"), def_arg(""), sensitivity(0)
{
    ZERO(pressed_cmd, INPUT_MAX_CODE);
    ZERO(released_cmd, INPUT_MAX_CODE);

    mtt = new res::Matter;
}

Avatar::~Avatar(void)
{
}

typedef res::Command<Avatar> SICmd;

const bool Avatar::init(Input* input_, VFS* vfs_)
{
    if (!input_ || !vfs_) { return false; }
    input = input_;

    hud = new HUD(vfs_);
    Put(hud);
    hud->init(this);

    picker = new Picker("Picker");
    picker->Add(vfs_->find("/System/Model/Ico"));
    picker->Add(new phy::Ray);
    Add(picker);

    Put(new Node("quit", new SICmd(this, &Avatar::cmd_quit)));
    Put(new Node("beacon", new SICmd(this, &Avatar::cmd_beacon)));
    Put(new Node("sensitivity", new SICmd(this, &Avatar::cmd_sensitivity)));
    Put(new Node("carrier", new SICmd(this, &Avatar::cmd_carrier)));

    Node* ctrl = new Node("Control");
    Put(ctrl);
    ctrl->SysAdd(new Node("Bind", new SICmd(this, &Avatar::cmd_bind), true));
    ctrl->SysAdd(new Node("Pointer", new SICmd(this, &Avatar::cmd_ptr), true));

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
    res::Node* off_toggle = carrier->find("-all");
    if (off_toggle) { off_toggle->exec(def_arg); }
}

void Avatar::set_carrier(Actor* carrier_)
{
    cerr << "carrier found: " << carrier_->get_name() << endl;
    carrier = carrier_;
//     carrier->Add(picker);
    carrier_->Add(this);
}

const bool Avatar::key(const InputCode pressed_[], const InputCode released_[])
{
    for (Size32 code_len = 0; pressed_[code_len] != K_NULL; ++code_len)
    {
        if (pressed_[code_len] == INPUT_MAX_CODE)
        { g_log->printf("invalid key pressed"); continue; }

        if (pressed_cmd[pressed_[code_len]] != NULL)
        { cerr << pressed_cmd[pressed_[code_len]]->exec(def_arg); }
        else { g_log->printf("Keycode %d is unbound", pressed_[code_len]); }
    }

    for (Size32 code_len = 0; released_[code_len] != K_NULL; ++code_len)
    {
        if (released_[code_len] == INPUT_MAX_CODE)
        { g_log->printf("invalid key released"); continue; }
        if (released_cmd[released_[code_len]] != NULL)
        { released_cmd[released_[code_len]]->exec(def_arg); }
    }

    return false;
}

const bool Avatar::input_ptr(const Int32 x_, const Int32 y_,
                             const Ratio frame_d_)
{
    picker->move_to(x_, y_);
    Mtx44 fps_mtx = picker->get_fps(sensitivity);
    Mtx44 norm_mtx = fps_mtx;

    norm_mtx.inverse();
    norm_mtx.rotate(-90, 90, 0);
    norm_mtx.pos(carrier->get_rel_pos());

    carrier->set_rel_mtx(norm_mtx);

//     carrier->set_dir(norm_mtx.get_quat());

    Vec3 c_pos = carrier->get_mtt()->get_c_mtx().get_pos();
    Vec3 n_pos = carrier->get_mtt()->get_n_mtx().get_pos();

    Vec3 si_pos = (n_pos - c_pos) * frame_d_ + c_pos;

    return false;
}

res::Entity* Avatar::pick_scene(const Int32 x_, const Int32 y_)
{
    picker->move_to(x_, y_);
    picker->screen_to_dir(Vec3(static_cast<Unit>(SCREEN_WIDTH),
                               static_cast<Unit>(SCREEN_HEIGHT)), 90);
//                           dynamic_cast<vid::RenderState*>(mtt)->get_fov());
/*
    std::cerr << "avatar dir: ";
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

const Unit Avatar::get_sensitivity(void)
{
    return sensitivity;
}

Unit Avatar::cmd_quit(const Str& arg_)
{
    g_finish = true;

    g_log->printf("quit");

    return 255;
}

Unit Avatar::cmd_beacon(const Str& arg_)
{
    return 0;
}

Unit Avatar::cmd_sensitivity(const Str& arg_)
{
    if (!arg_.empty())
    {
        istringstream ss(arg_);
        ss >> sensitivity;
        g_log->printf("sensitivity changed: %s", arg_.c_str());
    }
    return sensitivity;
}

Unit Avatar::cmd_carrier(const Str& arg_)
{
//     res::Entity* src = dynamic_cast<res::Entity*>(vfs->find(arg_));
//     if (src) { carrier = src; return 1; }
    return 0;
}

Unit Avatar::cmd_bind(const Str& arg_)
{
    Size32 key_len = arg_.find(CMD_SEPARATOR);

    if (key_len == Str::npos) { return 0; }
    // tokenize
    Str key_tok = arg_.substr(0, key_len);
    Size32 cmd_pos = arg_.find_first_not_of(CMD_SEPARATOR, key_len);

    if (cmd_pos == Str::npos) { return 0; }
    Str cmd_tok = arg_.substr(cmd_pos, arg_.length());

    Str cmd_name = cmd_tok;
    Size32 last_sep = cmd_tok.find_last_of(VFS_SEPARATOR);
    if (last_sep != Str::npos)
    { cmd_name = cmd_tok.substr(last_sep + 1, last_sep - cmd_tok.size()); }

    pressed_cmd[input->str_to_code(key_tok)] = carrier->find(cmd_tok);
    released_cmd[input->str_to_code(key_tok)] = NULL;

    if (!cmd_name.empty() && cmd_name.at(0) == '+') // toggle
    {
        Str toggleoff = cmd_tok.substr(0, last_sep + 1) + '-'
            + cmd_name.substr(1, cmd_name.size() - 1);
        released_cmd[input->str_to_code(key_tok)] = carrier->find(toggleoff);
    }

    return 0;
}

Unit Avatar::cmd_ptr(const Str& arg_)
{
//     cerr << "ptr: " << arg_ << endl;
    return 0;
}
