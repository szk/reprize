#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include "Widget.hpp"

namespace reprize
{
namespace inf
{
class Button : public Widget
{
public:
    Button(const Str& name_, const res::Material* bg_, const res::Font* fn_,
           const Vec3& pad_, const Align align_ = LEFT)
        : Widget(name_, bg_, fn_, pad_, align_) {}
    Button(const Button& src_)
        : Widget(src_)
        {}
    virtual ~Button(void) {}

    virtual res::Props* gen_props(void)
        {
            RE_PROPS_INHERIT(Button, Widget);
            RE_PROP_STR(Button, draggable);
            return RE_PROPS_FINISH;
        }

    virtual Button* Clone(const bool with_child_ = true)
        { return new Button(*this); }

    void set_draggable_area(const Vec3& area_) { draggable_area = area_; }
    virtual void pressed(const Vec3& pos_)
        { pressed_pos = rel_mtx.get_pos() - pos_; }
    virtual void released(const Vec3& pos_) {}

    virtual void hover(void)  // need keyframe action? like 'flash'
        {}

    virtual void activate(void)
        {
            // if button is draggable
//             if (draggable_area.is_origin() && prev_ptr_pos != pressed_pos)
//             { cmd->exec(cmd_arg); }
        }

    virtual void dragging(const Vec3& pos_)
        {
            if (draggable_area.is_origin()) { return; }
            else if (prev_ptr_pos != pos_) // use for 'draggable button.'
            {
                Vec3 next_pos = pos_ + pressed_pos;
                Unit right = draggable_area.get_x() - area.get_x(),
                    bottom = draggable_area.get_y() - area.get_y();
                if (next_pos.get_x() > right) { next_pos.set_x(right); }
                if (next_pos.get_y() > bottom) { next_pos.set_y(bottom); }
                if (next_pos.get_x() < 0) { next_pos.set_x(0); }
                if (next_pos.get_y() < 0) { next_pos.set_y(0); }

                repos(next_pos);
                prev_ptr_pos = next_pos;

//                 Vec3 bias((rel_mtx.get_pos()) / (draggable_area - area));
            }
        }

protected:
    const Str& prop_draggable(const Str& arg_)
        {
            if (arg_ == PROP_TOGGLE_ON)
            {
                Widget* parent_w = dynamic_cast<Widget*>(entry->Parent());
                if (parent_w != NULL)
                { draggable_area.set(parent_w->get_area()); }
            }
            else { draggable_area.set(Vec3(arg_)); }
            return RE_SUCCESS;
        }
    Vec3 pressed_pos, prev_ptr_pos, draggable_area;
};
}
}
#endif
