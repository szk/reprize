#include "Common.hpp"

#include "input/Keyboard.hpp"
#include "widget/Widget.hpp"
#include "widget/Caption.hpp"
#include "Theme.hpp"
#include "resource/Keyframe.hpp"
#include "Desktop.hpp"

using namespace reprize;
using namespace res;
using namespace inp;
using namespace inf;
using namespace vid;

Desktop::Desktop(const Str& name_, const Vec3 area_, const Material* wallpaper_,
                 const Font* fn_)
    : Widget(name_, wallpaper_, fn_, Vec3()), grabbed(NULL), focused(NULL),
      cursor(NULL), popupmenu(NULL)
{
    area = area_;
    resize(area);
    set_size_strained(true);
    DEL(mtt);
    rs_stack = new vid::RenderState(new res::Model("DesktopStack"),
                                    Mtx44(), Vec3(1.0, 1.0, 1.0), 1.0, Mtx44(),
                                    false, 0, vid::SB_MATERIAL, vid::RT_COLOR,
                                    0, 0, RS_BF_SRC_ALPHA |
                                    (RS_BF_ONE_MINUS_SRC_ALPHA << 4));
    mtt = rs_stack;
}

Desktop::Desktop(const Desktop& src_)
    : Widget(src_.name, src_.form_mdl->get_material(), src_.font, Vec3()),
      grabbed(src_.grabbed), focused(src_.focused), cursor(src_.cursor),
      popupmenu(src_.popupmenu)
{
    area = src_.area;
    resize(area);
    set_size_strained(true);
    DEL(mtt);
    rs_stack = new RenderState(new res::Model("DesktopStack"), Mtx44(),
                               Vec3(1.0, 1.0, 1.0),
                               1.0, Mtx44(), false, 0, vid::SB_REVERSE,
                               vid::RT_COLOR, RS_CLEAR_Z, 0,
                               RS_BF_SRC_ALPHA
                               | (RS_BF_ONE_MINUS_SRC_ALPHA << 4));

    mtt = rs_stack;
}

Desktop::~Desktop(void)
{
}

void Desktop::pressed(const Vec3& pos_)
{
    RE_DBG("pressed: %d, %d", pos_.get_x(), pos_.get_y());
}

void Desktop::released(const Vec3& pos_)
{
    RE_DBG("released: %d, %d", pos_.get_x(), pos_.get_y());
}

void Desktop::dragging(const Vec3& pos_)
{
    RE_DBG("dragged: %d, %d", pos_.get_x(), pos_.get_y());
}

void Desktop::enable(void)
{
}

void Desktop::disable(void)
{
}

const bool Desktop::Adopt(Node* node_)
{
    if (node_->get_name() == "SysCursor")
    {
        // XXX
        cursor = dynamic_cast<Widget*>(node_);
        win_ls.push_front(cursor);
        return Entity::Adopt(cursor);
    }
    else if (node_->get_name() == "SysPopupmenu")
    {
        // XXX
        popupmenu = dynamic_cast<Widget*>(node_);
        return Entity::Adopt(popupmenu);
    }

    Widget* widget = NULL;
    if (!(widget = dynamic_cast<Widget*>(node_))) { return false; }
    win_ls.push_front(widget);
    return Entity::Adopt(widget);
}

Desktop* Desktop::Clone(const bool with_child_)
{
    Desktop* current = new Desktop(*this);
//     for (First(); !IsDone(); Next())
//     { current->Add(Current()->Clone(true)); }
    return current;
}

const bool Desktop::pick_entity(const Int32 x_, const Int32 y_,
                                Entity* picked_)
{
    // think about press on the scene, or ui
    if (press && !pre_press && !grabbed) // pressed
    {
        rs_stack->debug();
        if ((grabbed_ent = picked_)) { popup(x_, y_, picked_); }
    }
    else if (!press && grabbed) // released
    {
        if (grabbed_ent == picked_) { focused_ent = grabbed_ent; }
        if (picked_ == NULL || grabbed_ent != picked_) { popdown(); }
        grabbed_ent = NULL;
    }
    else if ((ptr_pos != pre_ptr_pos) && grabbed_ent) // dragging
    { return true; }

    return false;
}

const bool Desktop::pick_widget(const Int32 x_, const Int32 y_)
{
    pre_ptr_pos = ptr_pos;
    ptr_pos.set(static_cast<Unit>(x_), static_cast<Unit>(y_), 0);

    return input_ptr(ptr_pos, pre_ptr_pos, press, pre_press);
}

const bool Desktop::input_ptr(const Vec3& ptr_pos_, const Vec3& pre_ptr_pos_,
                              const bool press_, const bool pre_press_)
{
    if (!cursor) { return false; }
    if (ptr_pos_ != pre_ptr_pos_)
    {
        Mtx44 desktop_mtx = mtt->get_c_mtx();
        desktop_mtx.mlt(ptr_pos_);
        cursor->repos(desktop_mtx.get_pos());
    }
    if (is_on(ptr_pos_) == this && !grabbed) { return false; }

    for (std::list<Widget*>::iterator win_itr = win_ls.begin();
         win_itr != win_ls.end(); ++win_itr)
    {
        if (check((*win_itr), ptr_pos_, pre_ptr_pos_, press_, pre_press_))
        {
            rs_stack->splice_tail((*win_itr)->get_mtt());
            win_ls.push_front(*win_itr);
            win_itr = win_ls.erase(win_itr);
            break;
        }
    }

    rs_stack->splice_tail(cursor->get_mtt());

    return true;
}

const bool Desktop::key(InputBuffer& pressed_, InputBuffer& released_)
{
    bool blink = false;
    pre_press = press;

    for (pressed_.First(); !pressed_.IsDone(); pressed_.Next())
    {
        if (pressed_.Current() == M_BUTTON0)
        {
            blink = press = true;
        }
        else { typed(pressed_.Current()); }
    }

    for (released_.First(); !released_.IsDone(); released_.Next())
    {
        if (released_.Current() == M_BUTTON0)
        {
            press = false;
            once = blink;
        }
    }
    return true;
}

const Vec3& Desktop::get_ptr_pos(void)
{
    return ptr_pos;
}

void Desktop::typed(const InputCode key_)
{
    if (focused && focused != this) { focused->typed(key_); }
}

void Desktop::str(const Char* str_)
{
    if (focused && focused != this && str_[0] != NULCHAR)
    { focused->str(str_); }
}

const bool Desktop::check(Widget* current_, const Vec3& pos_,
                          const Vec3& pre_pos_, const bool press_,
                          const bool pre_press_)
{
    if (press_ && !pre_press_ && !grabbed) // pressed
    {
        if ((grabbed = current_->is_on(pos_)))
        {
            if (grabbed == win_ls.back()) { grabbed->pressed(pos_); }
            else
            {
                // set this widget on the top of widget stack
                grabbed->pressed(pos_);
                grabbed->get_mtt();
//                 grabbed->set_keyframe(KF_BEHV_PLAY_SOUND, theme->get_pressed_snd());
                return true;
            }
        }
    }
    else if (!press_ && grabbed) // released
    {
        if (grabbed == current_->is_on(pos_))
        {
            grabbed->activate();
            focused = grabbed;
//             grabbed->set_keyframe(KF_BEHV_PLAY_SOUND, theme->get_released_snd());
        }
        grabbed->released(pos_);
        grabbed = NULL;
    }
    else if ((pos_ != pre_pos_) && grabbed) // dragging
    {
        grabbed->dragging(pos_);
        // dragging sound
        // current_->set_keyframe(KF_BEHV_PLAY_SOUND, theme->get_dragging_snd());
    }

    return false;
}

void Desktop::show(void)
{
    if (!cursor) { return; }

//     RE_DBG("show: %f, %f",
//            cursor->get_rel_pos().get_x(), cursor->get_rel_pos().get_y());

//     cursor->set_keyframe(KF_PROP_ALPHA, true, 10, Vec3(1, 1, 1));
    for (std::list<Widget*>::iterator itr = win_ls.begin();
         itr != win_ls.end(); ++itr)
    {
        (*itr)->enable();
//         (*itr)->set_keyframe(KF_PROP_COORDINATE, true, 5, win_poss.top(), true);
        (*itr)->set_keyframe(KF_PROP_ALPHA, true, 10, Vec3(1, 1, 1), true);
        win_poss.pop();
    }
}

void Desktop::hide(void)
{
    if (!cursor) { return; }
//     RE_DBG("hide: %f, %f",
//            cursor->get_rel_pos().get_x(), cursor->get_rel_pos().get_y());

    for (std::list<Widget*>::iterator itr = win_ls.begin();
         itr != win_ls.end(); ++itr)
    {
        Unit win_height = (*itr)->get_bound().get_y();
        Vec3 pos = (*itr)->get_rel_mtx().get_pos();

        Unit center_y = win_height / 2 + pos.get_y();
        Unit dst_y = -win_height;

        if (center_y > SCREEN_HEIGHT / 2) { dst_y = SCREEN_HEIGHT; }
        (*itr)->disable();

//         if (cursor == (*itr))
        { (*itr)->set_keyframe(KF_PROP_ALPHA, true, 10, Vec3(0, 0, 0), true); }
//         else
//         {
//             (*itr)->set_keyframe(KF_PROP_COORDINATE, true, 5,
//                                  Vec3(pos.get_x(), dst_y, 0, true));
//         }
        win_poss.push(pos);
    }
}

void Desktop::popup(const Int32 x_, const Int32 y_, Entity* src_)
{
//             popupmenu->get_mtt()->show();
    popupmenu->repos(Vec3(static_cast<Unit>(x_), static_cast<Unit>(y_), 0));
    Caption* line = new Caption("PopupMenu");
//     line->write(src_->get_name(), font);
    popupmenu->Add(line);
//             for (src_->First(); !src_->IsDone(); src_->Next())
//             {
//             }

    if (!src_) { return; }
    RE_DBG("popupmenu: %s", popupmenu->get_name().c_str());
}

void Desktop::popdown(void)
{
//             Separate(popupmenu);
//             popupmenu->get_mtt()->hide();
    RE_DBG("popdown: %s", popupmenu->get_name().c_str());
}

// XXX evil
Node* Desktop::Current(void)
{
    if (win_ls_itr == win_ls.end()) { return NULL; }
    return *win_ls_itr;
}

void Desktop::First(void)
{
    win_ls_itr = win_ls.begin();
    reached = false;
}

void Desktop::Next(void)
{
    if (win_ls_itr == win_ls.end()) { reached = true; return; }
    ++win_ls_itr;
}

const bool Desktop::IsDone(void) const
{
    return reached;
}

typedef PropStr<Desktop> DTPropStr;

const Str& Desktop::prop_show(const Str& arg_)
{
    if (arg_.empty()) { show(); }
    Widget* tgt = dynamic_cast<Widget*>(find(arg_));
    if (tgt == NULL) { return RE_FAILURE; }
    tgt->set_keyframe(KF_PROP_COORDINATE, true, 10, Vec3(0, 100));
    return RE_SUCCESS;
}

const Str& Desktop::prop_hide(const Str& arg_)
{
    if (arg_.empty()) { hide(); }
    Widget* tgt = dynamic_cast<Widget*>(find(arg_));
    if (tgt == NULL) { return RE_FAILURE; }

    tgt->set_keyframe(KF_PROP_COORDINATE, true, 10, Vec3(0, 100));
    tgt->set_keyframe(KF_PROP_ALPHA, true, 10, Vec3(0, 0));
    return RE_SUCCESS;
}
