#ifndef DESKTOP_HPP_
#define DESKTOP_HPP_

#include "video/RenderState.hpp"
#include "widget/Widget.hpp"

namespace reprize
{
namespace inf
{
class Theme;

class Desktop : public Widget
{
public:
    Desktop(const Str& name_, const Vec3 area_,
            const res::Material* wallpaper_, const res::Font* fn_);
    Desktop(const Desktop& src_);
    virtual ~Desktop(void);

    virtual void pressed(const Vec3& pos_);
    virtual void released(const Vec3& pos_);
    virtual void dragging(const Vec3& pos_);

    void enable(void);
    void disable(void);

    virtual Desktop* Clone(const bool with_child_ = true);

    const bool pick_entity(const Int32 x_, const Int32 y_,
                           res::Entity* picked_);
    const bool pick_widget(const Int32 x_, const Int32 y_);

    const bool input_ptr(const Vec3& ptr_pos_, const Vec3& pre_ptr_pos_,
                         const bool press_, const bool pre_press_);
    const bool key(inp::InputBuffer& pressed_, inp::InputBuffer& released_);
    const Vec3& get_ptr_pos(void);

    void typed(const InputCode key_);
    void str(const Char* str_);

    const bool check(Widget* current_, const Vec3& pos_, const Vec3& pre_pos_,
                     const bool press_, const bool pre_press_);
    void show(void);
    void hide(void);
    void popup(const Int32 x_, const Int32 y_, res::Entity* src_);
    void popdown(void);
    Node* Current(void);
    void First(void);
    void Next(void);
    const bool IsDone(void) const;

protected:
    const Str& prop_show(const Str& arg_);
    const Str& prop_hide(const Str& arg_);

    Widget* grabbed,* focused,* cursor,* popupmenu;
    Entity* grabbed_ent,* focused_ent;
    vid::RenderState* rs_stack;
    std::list<Widget*> win_ls;
    std::list<Widget*>::iterator win_ls_itr;

    std::stack<Vec3> win_poss;
    Vec3 ptr_pos, pre_ptr_pos, u_mask_pos, l_mask_pos;
    bool click, press, pre_press, once, reached;
private:
    virtual const bool Adopt(Node* node_);
};
}
}

#endif
