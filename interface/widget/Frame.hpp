#ifndef FRAME_HPP_
#define FRAME_HPP_

#include "Widget.hpp"
#include "video/RenderState.hpp"

namespace reprize
{
namespace inf
{
class Frame : public Widget
{
public:
    Frame(const Str& name_, const res::Material* bg_, const res::Font* fn_,
          const Vec3& pad_)
        : Widget(name_, bg_, fn_, pad_), content(NULL)
        {}
    Frame(const Frame& src_)
        : Widget(src_), content(NULL)
        {}
    virtual ~Frame(void) {}

    virtual Frame* Clone(const bool with_child_ = true)
        {
            return new Frame(*this);
        }

    const bool add_content(Widget* child_)
        {
            if (child_ == NULL) { return false; }

            child_topleft.set(0, area.get_y(), 0);
            child_->repos(child_topleft);
            Entity::Add(child_);

            content = child_;
            if (content->rs_is_correct())
            { RE_DBG("%s :rs[correct]", child_->get_name().c_str()); }
            return true;
        }
    const bool set_title(const Str& title_)
        {
            if (!is_valid_name(title_)) { return false; }
            name = title_;
            return true;
        }
    Widget* get_content(void) { return content; }

    virtual const Vec3& get_bound(void)
        {
            if (content == NULL) { return area; }
            whole_area.set(content->get_area().get_x(),
                           area.get_x() + content->get_area().get_y(), 0);
            return whole_area;
        }

    virtual void activate(void) {}

protected:
    Widget* content;
    Vec3 whole_area;
private:
    const bool Adopt(Node* node_)
        {
            if (mtt) { RE_DBG("put: %s", node_->get_name().c_str()); }
            else { RE_DBG("not put: %s", node_->get_name().c_str()); }
            return Widget::Adopt(node_);
        }

};
}
}
#endif
