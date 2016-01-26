#ifndef CHECKBOX_HPP_
#define CHECKBOX_HPP_

#include "Button.hpp"

namespace reprize
{
namespace inf
{
class CheckBox : public Button
{
public:
    CheckBox(const Str& name_, const res::Material* bg_,
             const res::Font* fn_, const Vec3& pad_)
        : Button(name_, bg_, fn_, pad_), box_pad(pad_), box(NULL), mark(NULL)
        {}
    CheckBox(const CheckBox& src_)
        : Button(src_), box_pad(src_.box_pad), box(NULL), mark(NULL) {}
    virtual ~CheckBox(void) {}

    virtual CheckBox* Clone(const bool with_child_ = true)
        {
            return new CheckBox(*this);
        }

    void appendix(const Str& arg_, res::Props* props_)
        {
            box = new Button("Check", form_mdl->get_material(), font, box_pad);
            Widget::Add(box);
            box->appendix(RE_CHECKED, NULL); // XXX this will be a glyph
            mark = dynamic_cast<Caption*>(box->find("Caption"));
            box->set_event_transparent(true);

            child_topleft.add(box_pad.get_x(), 0, 0);
            set_alpha(0);
            add_caption(arg_);
            box->rearrange();
        }

    virtual void pressed(const Vec3& pos_) {}
    virtual void released(const Vec3& pos_) {}
    virtual void hover(void) {} // need keyframe action? like 'flash'
    virtual void activate(void)
        {
            if (mark) { mark->setf(font, 0, ""); }
            if (property) { property->Apply(PROP_TOGGLE_OFF); }
        }

protected:
    Widget* box;
    Caption* mark;
    Vec3 box_pad;
};
}
}
#endif
