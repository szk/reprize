#ifndef CURSOR_HPP_
#define CURSOR_HPP_

#include "Widget.hpp"
#include "video/RenderState.hpp"

namespace reprize
{
namespace inf
{
class Cursor : public Widget
{
public:
    Cursor(const Str& name_, const res::Material* bg_, const res::Font* fn_,
           const Vec3& pad_)
        : Widget(name_, bg_, fn_, pad_)
        {}
    Cursor(const Cursor& src_)
        : Widget(src_)
        {}
    virtual ~Cursor(void) {}

    virtual Cursor* Clone(const bool with_child_ = true)
        { return new Cursor(*this); }

    virtual const bool resize(const Vec3& size_)
        {
            Widget::resize(form_mdl->get_material()->get_size());
            return true;
        }
    virtual const bool repos(const Vec3& pos_)
        {
            Widget::repos(pos_);
            return false;
        }

protected:
};
}
}
#endif
