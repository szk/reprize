#ifndef POINTER_HPP_
#define POINTER_HPP_

#include "input/InputBase.hpp"
#include "input/InputBuffer.hpp"

namespace reprize
{
namespace inf
{
enum GESTURE
{
    GESTURE_MOVE_X,
    GESTURE_MOVE_Y,
    GESTURE_MOVE_Z,
    GESTURE_CLICK,
    GESTURE_TAP,
    GESTURE_PINCH,
    GESTURE_ZOOM,
    GESTURE_NUM,
};

class Pointer : public res::Node
{
public:
    Pointer(void) : Node("Pointer")
        {
            RE_ZERO(begin_cmd, GESTURE_NUM);
            RE_ZERO(finish_cmd, GESTURE_NUM);
        }
    virtual ~Pointer(void) { DEL(entry); }

    virtual const bool init(inp::Input* input_)
        {
            input = input_;
            return true;
        }

    virtual res::Props* gen_props(void)
        {
            RE_PROPS_START(Pointer);
            RE_PROP_STR(Pointer, sensitivity);
            RE_PROP_STR(Pointer, pitch);
            RE_PROP_STR(Pointer, yaw);
            RE_PROP_STR(Pointer, invert);
            return RE_PROPS_FINISH;
        }

    const bool set_prop(res::Props* pr_)
        {
            pr = pr_;
            return false;
        }

    const bool gesture(GESTURE ges_, const bool begin_)
        {
            if (ges_ == GESTURE_NUM) { return false; }

            if (begin_ && begin_cmd[ges_] != NULL)
            { begin_cmd[ges_]->Apply(RE_SUCCESS); }

            if (!begin_ && finish_cmd[ges_] != NULL)
            { finish_cmd[ges_]->Apply(RE_SUCCESS); }

            return true;
        }

private:
    const Str& prop_gesture(const Str& arg_)
        {
        }
    const Str& prop_sensitivity(const Str& arg_)
        {
            if (!arg_.empty())
            {
                ISStrm ss(arg_);
                ss >> sensitivity;
                RE_DBG("changed: %f", sensitivity);
            }
            return RE_SUCCESS;
        }

    const Str& prop_yaw(const Str& arg_)
        {
            return RE_FAILURE;
        }

    const Str& prop_pitch(const Str& arg_)
        {
            return RE_FAILURE;
        }
    const Str& prop_invert(const Str& arg_)
        {
            return RE_FAILURE;
        }

    inp::Input* input;
    res::Props* pr;
    res::Property* begin_cmd[GESTURE_NUM],* finish_cmd[GESTURE_NUM];
    Unit sensitivity;
};

}
}

#endif
