#ifndef WIRING_HPP_
#define WIRING_HPP_

#include "Button.hpp"
#include "Caption.hpp"

namespace reprize
{
namespace inf
{
class Wiring : public Widget
{
public:
    Wiring(const Str& name_, const res::Material* bg_,
         const res::Font* fn_, const Vec3& pad_)
        : Widget(name_, bg_, fn_, pad_)
        { /* ::set(content, fn_); */ }
    Wiring(const Wiring& src_)
        : Widget(src_) {}
    virtual ~Wiring(void) {}

    virtual Wiring* Clone(const bool with_child_ = true)
        {
            return new Wiring(*this);
        }

    virtual void pressed(const Vec3& pos_)
        {
            pressed_pos = pos_;
            pressed_pos = rel_mtx.get_pos() - pressed_pos;
        }
    virtual void released(const Vec3& pos_)
        {
        }

    virtual void hover(void)  // need keyframe action? like 'flash'
        {
        }

    virtual void activate(void)
        {
            RE_DBG("Activate wiring system");
        }

    virtual void dragging(const Vec3& pos_)
        {
        }

protected:
    Vec3 pressed_pos;
    Str content;
    bool updated;
};
}
}

#endif

