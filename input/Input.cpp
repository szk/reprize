#include "Common.hpp"

#include "InputBase.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "OptIO.hpp"

using namespace reprize;
using namespace std;
using namespace inp;

Input::Input(void)
    : node(NULL)
{
    pvt = new inp_pvt;
    node = new res::Node("Input");
}

Input::~Input(void)
{
    DEL(pvt->base);
    DEL(pvt->keyboard);
    DEL(pvt->mouse);
    DEL(pvt->opt_io);
    DEL(pvt);
    DEL(node);
}

res::Node* Input::get_node(void)
{
    return node;
}

void Input::set_view(inf::Interface* inf_)
{
    pvt->inf = inf_;
}

void Input::set_base(InputBase* base_)
{
    pvt->base = base_;
}

void Input::set_keyboard(Keyboard* keyboard_)
{
    pvt->keyboard = keyboard_;
}

void Input::set_mouse(Mouse* mouse_)
{
    pvt->mouse = mouse_;
}

void Input::set_opt_io(OptIO* opt_io_)
{
    pvt->opt_io = opt_io_;
}

typedef res::PropStr<Input> InpPropStr;

const bool Input::init(void)
{
    return pvt->base->init() && pvt->keyboard->init()
        && pvt->mouse->init() && pvt->opt_io->init();
}

void Input::restore(void)
{
    pvt->base->restore();
    pvt->keyboard->restore();
    pvt->mouse->restore();
    pvt->opt_io->restore();
}

void Input::check(void)
{
    pvt->base->check(pvt->keyboard, pvt->mouse, pvt->opt_io);
}

void Input::reset(void)
{
    pvt->base->reset();
    pvt->keyboard->reset();
    pvt->mouse->reset();
    pvt->opt_io->reset();
}

void Input::set_ptr_relative(const bool relative_)
{
    pvt->mouse->set_relative(relative_);
}

const bool Input::set_ptr(const uInt32 scr_x_, const uInt32 scr_y_)
{
    return pvt->mouse->set_pos(scr_x_, scr_y_);
}

const InputCode Input::str_to_code(const Str& key_)
{
    if (RE_ISNUM(key_))
    {
        InputCode code;
        stringstream ss(key_);
        ss >> code;
        return code;
    }
    return pvt->base->str_to_code(key_);
}

const Str& Input::code_to_name(const InputCode code_)
{
    return pvt->base->code_to_name(code_);
}

InputBuffer& Input::get_pressed(void)
{
    return pvt->base->get_pressed();
}

InputBuffer& Input::get_released(void)
{
    return pvt->base->get_released();
}

Int32 Input::get_ptr_x(void)
{
    return pvt->mouse->get_x();
}

Int32 Input::get_ptr_y(void)
{
    return pvt->mouse->get_y();
}

InputCode Input::get_mod(void)
{
    return pvt->keyboard->get_mod();
}

const Char* Input::get_str(void)
{
    return pvt->keyboard->get_str();
}

const Str& Input::prop_code_to_name(const Str& arg_)
{
    InputCode code;
    stringstream ss(arg_);
    ss >> code;
    return pvt->base->code_to_name(code);
}
