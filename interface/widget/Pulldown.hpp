#ifndef PULLDOWN_HPP_
#define PULLDOWN_HPP_

#include "resource/PropStr.hpp"
#include "Button.hpp"
#include "Caption.hpp"

namespace reprize
{
namespace inf
{
class Pulldown : public Widget
{
    typedef res::PropStr<Pulldown> PDPropStr;

public:
    Pulldown(const Str& name_, const res::Material* bg_,
             const res::Font* fn_, const Vec3& pad_)
        : Widget(name_, bg_, fn_, pad_), toggled(false)
        { /* ::set(content, fn_); */ }
    Pulldown(const Pulldown& src_)
        : Widget(src_), toggled(false) {}
    virtual ~Pulldown(void) {}

    virtual  Pulldown* Clone(const bool with_child_ = true)
        {
            return new Pulldown(*this);
        }
    void init(Node* cmd_, const Str& def_arg_, const Align align_ = TOP)
        {
            pulldown_root = cmd_;
            res::Props* pr = new res::Props;
            pr->Add("toggle", new PDPropStr(this, &Pulldown::prop_toggle));
            btn_root = new Button("Button", form_mdl->get_material(), font,
                                  Vec3(10, 10));
            Caption* btn_label = new Caption("Label");
//   XXX          btn_root->init(toggle_cmd, cmd_->get_name());
//             btn_label->write(cmd_->get_name(), font);
            btn_root->Add(btn_label);
            Widget::Add(btn_root);
        }

    virtual void pressed(const Vec3& pos_) {}
    virtual void released(const Vec3& pos_) {}

    virtual void hover(void) {}

    virtual void activate(void)
        {
//                 cmd->exec(cmd_arg);
            RE_DBG("Activate");
        }

    virtual void dragging(const Vec3& pos_) {}

protected:
    const Str& prop_toggle(const Str& arg_)
        {
            Node* node = pulldown_root->find(arg_);
            if (arg_ == pulldown_root->get_name()) { node = pulldown_root; }

            if (node == NULL)
            {
                RE_DBG("Pulldown: not found: %s", arg_.c_str());
                return RE_FAILURE;
            }

            for (node->First(); !node->IsDone(); node->Next())
            {
                Button* btn = new Button(node->Current()->get_name(),
                                         form_mdl->get_material(), font,
                                         Vec3());
                Caption* btn_line = new Caption("Line");

                Str dir_name = node->get_name() + VFS_NODE_SEPARATOR
                    + node->Current()->get_name();
//                 btn->init(toggle_cmd, dir_name);
//                 btn_line->write(node->Current()->get_name(), font);
                btn->Add(btn_line);

                Node* btn_node = btn_root->find(node->get_name());
                if (btn_node)
                { RE_DBG("%s is not null", dir_name.c_str()); }
                else
                { RE_DBG("%s is null", dir_name.c_str()); }
                Widget::Add(btn);
            }

            return RE_FAILURE;
        }

    res::Node* pulldown_root,* toggle_cmd;
    Button* btn_root;
    Vec3 pressed_pos;
    bool toggled;
};
}
}

#endif

