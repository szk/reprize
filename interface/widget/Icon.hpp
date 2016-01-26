#ifndef ICON_HPP_
#define ICON_HPP_

#include "Widget.hpp"

namespace reprize
{
namespace res { class Node; class Texture; class Font; }

namespace inf
{
class Icon : public Widget
{
public:
    Icon(const Str& name_, const res::Material* bg_, const res::Font* fn_,
         const Vec3& pad_)
        : Widget(name_, bg_, fn_, pad_)
        {}
    Icon(const Icon& src_) : Widget(src_) {}
    virtual ~Icon(void) {}

    virtual Icon* Clone(const bool with_child_ = true)
        {
            return new Icon(*this);
        }

    virtual void pressed(const Vec3& pos_)
        {
            Vec3 pressed = pos_;
            pressed = rel_mtx.get_pos() - pressed;
            pressed_x = pressed.get_x();
            pressed_y = pressed.get_y();
        }
    virtual void released(const Vec3& pos_)
        {
        }
    virtual void activate(void)
        {
            RE_DBG("%s", cmd_str.c_str());
        }
    virtual void dragging(const Vec3& pos_)
        {
//            Vector3<Unit> dst_pos(x_ - pressed_x, y_ - pressed_y, 0);
//            repos(dst_pos);
//            RE_DBG("d: %s %f, %f", name.c_str(), dst_pos.get_x(), dst_pos.get_y());
        }

    const Str& get_cmd_str(void) { return cmd_str; }

protected:
    Unit pressed_x, pressed_y;
    Str cmd_str;
};
}
}
#endif
