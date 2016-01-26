#include "Common.hpp"

#include "resource/Geometry.hpp"
#include "resource/Texture.hpp"

#include "resource/Entity.hpp"
#include "resource/Font.hpp"
#include "resource/Keyframe.hpp"

#include "input/Mouse.hpp"

#include "widget/Widget.hpp"
#include "widget/Frame.hpp"
#include "widget/Div.hpp"

#include "widget/Button.hpp"
#include "widget/Canvas.hpp"
#include "widget/Caption.hpp"
#include "widget/CheckBox.hpp"
#include "widget/TextBox.hpp"
#include "widget/Icon.hpp"
#include "widget/KeyBox.hpp"
#include "widget/LogBox.hpp"
#include "widget/ScrollBar.hpp"
#include "widget/TabBox.hpp"
#include "widget/Pulldown.hpp"
#include "widget/Slider.hpp"
#include "widget/Thumb.hpp"
#include "widget/Wiring.hpp"

#include "widget/Window.hpp"
#include "widget/Cursor.hpp"

#include "Theme.hpp"

using namespace reprize;
using namespace std;
using namespace inf;
using namespace res;

Theme::Theme(const Str& name_, const Material* bg_, const Font* fn_,
             const Font* mi_fn_, const res::Sound* pressed_snd_,
             const res::Sound* released_snd_, const res::Sound* dragging_snd_)
    : Node(name_), fallback(NULL), bg(bg_), fn(fn_), mi_fn(mi_fn_),
      pressed_snd(pressed_snd_), released_snd(released_snd_),
      dragging_snd(dragging_snd_)
{
    /* TODO */
    fallback = new Widget("Fallback", bg, fn, Vec3(4, 4));
    if (bg && fn) { /* TODO */ }
    else { /* TODO */ }
    Add(fallback);

    Add(new Button("Button", bg, fn, Vec3(4, 4)));
    Add(new Canvas("Canvas", bg, fn, Vec3(4, 4)));
    Add(new CheckBox("CheckBox", bg, fn, Vec3(4, 4)));
    Add(new Cursor("Cursor", bg, fn, Vec3()));
    Add(new Div("Div", NULL, fn_, Vec3()));
    Add(new TextBox("TextBox", bg, fn, Vec3(4, 4)));
    Add(new KeyBox("KeyBox", bg, fn, Vec3(4, 4)));
    Add(new LogBox("LogBox", bg, fn, Vec3(4, 4)));
    Add(new Frame("Frame", bg, fn, Vec3(6, 6)));
    Add(new ScrollBar("ScrollBar", bg, fn, Vec3(6, 6)));
    Add(new Slider("Slider", bg, mi_fn_, Vec3(4, 4)));
    TabBox* tabbox = new TabBox("TabBox", bg, fn, Vec3(4, 4));
    Add(tabbox);
    Thumb* thumb = new Thumb("Thumb", bg, fn, Vec3(4, 4));
    Add(thumb);
    tabbox->set_tab(thumb);
    Add(new Pulldown("Pulldown", bg, fn, Vec3(4, 4)));
    Add(new Caption("Caption"));
    Add(new Window("Window", bg, fn, Vec3(4, 4)));

    if (pressed_snd != NULL)
    { RE_DBG("press snd: %s", pressed_snd->get_name().c_str()); }
    if (released_snd != NULL)
    { RE_DBG("release snd: %s", released_snd->get_name().c_str()); }
    if (dragging_snd != NULL)
    { RE_DBG("drag snd: %s", dragging_snd->get_name().c_str()); }
}

Theme::Theme(const Theme& src_)
    : Node(src_.name), fallback(NULL), bg(src_.bg), fn(src_.fn),
      mi_fn(src_.mi_fn),
      pressed_snd(src_.pressed_snd), released_snd(src_.released_snd),
      dragging_snd(src_.dragging_snd)
{
    /* TODO */
    fallback = new Widget("Fallback", bg, fn, Vec3(4, 4));
    if (bg && fn) { /* TODO */ }
    else { /* TODO */ }
    Add(fallback);
}

Theme::~Theme(void)
{
}

Theme* Theme::Clone(const bool with_child_)
{
    return new Theme(*this);
}

const Sound* Theme::get_pressed_snd(void)
{
    return pressed_snd;
}

const Sound* Theme::get_released_snd(void)
{
    return released_snd;
}

const Sound* Theme::get_dragging_snd(void)
{
    return dragging_snd;
}
