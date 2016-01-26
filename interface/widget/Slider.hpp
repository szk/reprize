#ifndef SLIDER_HPP_
#define SLIDER_HPP_

#include "Button.hpp"

namespace reprize
{
namespace inf
{
class Slider : public Button
{
public:
    Slider(const Str& name_, const res::Material* bg_,
           const res::Font* fn_, const Vec3& pad_)
        : Button(name_, bg_, fn_, pad_)
        {}
    Slider(const Slider& src_)
        : Button(src_) {}
    virtual ~Slider(void) {}

    virtual Slider* Clone(const bool with_child_ = true)
        {
            return new Slider(*this);
        }

   void appendix(const Str& arg_, res::Props* props_)
        {
            knob = new Button("Knob", form_mdl->get_material(), font, Vec3());
            Widget::Add(knob);
            knob->resize(form_mdl->get_material()->get_size());

            knob->appendix(RE_CHECKED, NULL); // XXX this will be a glyph
//             knob->set_event_transparent(true);

            knob->set_draggable_area(Vec3(area.get_x(), area.get_y()));

//             child_topleft.add(box_pad.get_x(), 0, 0);
//             set_alpha(0);
//             add_caption(arg_);
//             box->rearrange();
        }
/*
    void init(Node* cmd_, const Str& prop_arg_, const Align align_ = TOP)
        {
            knob = 
//             knob->init(cmd_, prop_arg_, align_);set the cmd at this point
            knob->resize(form_mdl->get_material()->get_size());

            // caption = new Caption("00", form_mdl->get_material(), font, 0, 0);
            // caption->write(Str("00"), font);

            // knob->Add(caption);

            std::stringstream ss;
            ss << area.get_x() << " " << area.get_y() + pad.get_x() << " " << 0;
//             knob->find("draggable")->exec(ss.str());

            bool v_margin = knob->get_area().get_x() > area.get_x();
            bool h_margin = knob->get_area().get_y() > area.get_y();
            Widget::Add(knob);
            resize(Vec3(area.get_x() - v_margin * pad.get_y() * 2,
                        area.get_y() - h_margin * pad.get_x() * 2));
            knob->set_draggable_area(Vec3(area.get_x(), area.get_y()));
            knob->repos(Vec3());

            // we need to get range or something from xml_->get_att("range")
            // if you want to 'drag to exec some command', you can add cmd to Button::dragging
        }
*/

    virtual void pressed(const Vec3& pos_)
        {
            Vec3 knob_center = knob->get_area() / 2;
            knob->pressed(mtt->get_c_mtx().get_pos()
                          + knob->get_rel_mtx().get_pos() + knob_center);
            dragging(pos_);
        }

    virtual void released(const Vec3& pos_)
        {
        }

    virtual void dragging(const Vec3& pos_) { knob->dragging(pos_); }
    virtual void typed(const InputCode key_)
        {
            Vec3 pos = knob->get_mtt()->get_c_mtx().get_pos();
            RE_DBG("slider");
            switch (key_)
            {
            case K_UP: RE_DBG("up");
                dragging(Vec3(0, -5, 0));
                break;
            case K_LEFT: RE_DBG("left");
                dragging(Vec3(-5, 0, 0));
                break;
            case K_RIGHT: RE_DBG("right");
                dragging(Vec3(5, 0, 0));
                break;
            case K_DOWN: RE_DBG("down");
                dragging(Vec3(0, 5, 0));
                break;
            }
        }
    virtual void eval(const res::Matter* parent_mtt_)
        {
            Entity::eval(parent_mtt_);
/*
            if (knob->get_rel_pos() != prev_knob_pos)
            {
                prev_knob_pos = knob->get_rel_pos();
                Vec3 ratio = prev_knob_pos / (area - knob->get_area());
                std::stringstream ss;
                ss << " " << ratio.get_x() << " " << ratio.get_y();
//                 caption->setf(font, 0, ss.str().c_str());
//                 cmd_arg = ss.str();
                activate();
                }
*/
        }

protected:
    Button* knob;
    Caption* caption;
    Vec3 prev_knob_pos;
};
}
}
#endif
