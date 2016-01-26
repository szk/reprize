#ifndef SCROLL_BAR_HPP_
#define SCROLL_BAR_HPP_

#include "Button.hpp"

namespace reprize
{
namespace inf
{
class ScrollBar : public Button
{
public:
    ScrollBar(const Str& name_, const res::Material* bg_,
              const res::Font* fn_, const Vec3& pad_)
        : Button(name_, bg_, fn_, pad_)
        {}
    ScrollBar(const ScrollBar& src_)
        : Button(src_) {}
    virtual ~ScrollBar(void) {}

    virtual  ScrollBar* Clone(const bool with_child_ = true)
        {
            return new ScrollBar(*this);
        }
protected:
    uInt32 viewport_x, viewport_y;
};
}
}
#endif
