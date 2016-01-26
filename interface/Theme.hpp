#ifndef THEME_HPP_
#define THEME_HPP_

#include "widget/Widget.hpp"

namespace reprize
{
namespace inf
{
class Theme : public res::Node
{
public:
    Theme(const Str& name_, const res::Material* bg_, const res::Font* fn_,
          const res::Font* mi_fn_, const res::Sound* pressed_snd = NULL,
          const res::Sound* released_snd = NULL,
          const res::Sound* dragging_snd = NULL);
    Theme(const Theme& src_);
    virtual ~Theme(void);

    virtual Theme* Clone(const bool with_child_ = true);

    const res::Sound* get_pressed_snd(void);
    const res::Sound* get_released_snd(void);
    const res::Sound* get_dragging_snd(void);

protected:
    Widget* fallback;
    const res::Material* bg;
    const res::Font* fn,* mi_fn;
    const res::Sound* pressed_snd,* released_snd,* dragging_snd;
};
}
}
#endif
