#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "Button.hpp"
#include "Caption.hpp"
#include "../Manipulator.hpp"

namespace reprize
{
namespace inf
{
class Window : public Button
{
public:
    Window(const Str& name_, const res::Material* bg_,
           const res::Font* fn_, const Vec3& pad_)
        : Button(name_, bg_, fn_, pad_), updated(false)
        {
            mtt = new vid::RenderState(form_mdl, Mtx44(), Vec3(1.0, 1.0, 1.0),
                                       1.0, Mtx44(), false, 0, vid::SB_REVERSE,
                                       vid::RT_COLOR, 0, 0, RS_BF_SRC_ALPHA |
                                       (RS_BF_ONE_MINUS_SRC_ALPHA << 4));
        }
    Window(const Window& src_)
        : Button(src_), updated(false)
        {
            mtt = new vid::RenderState(form_mdl, Mtx44(), Vec3(1.0, 1.0, 1.0),
                                       1.0, Mtx44(), false, 0, vid::SB_REVERSE,
                                       vid::RT_COLOR, 0, 0, RS_BF_SRC_ALPHA |
                                       (RS_BF_ONE_MINUS_SRC_ALPHA << 4));
        }
    virtual ~Window(void) {}

    virtual Window* Clone(const bool with_child_ = true)
        { return new Window(*this); }

protected:
    bool updated;
};
}
}

#endif

