#ifndef WIDGET_HPP_
#define WIDGET_HPP_

#include "resource/Entity.hpp"

namespace reprize
{
namespace res { class Texture; class Geometry; class Font; }

namespace inf
{
enum Align { TOP, MIDDLE, BOTTOM, LEFT, CENTER, RIGHT, ALIGN_NUM };

class Widget : public res::Entity
{
public:
    Widget(const Str& name_, const res::Material* bg_ = NULL,
           const res::Font* fn_ = NULL, const Vec3& pad_ = Vec3(4, 4),
           const Align align_ = LEFT);
    Widget(const Widget& src_);
    virtual ~Widget(void);

    virtual Widget* Clone(const bool with_child_ = true);

    virtual void appendix(const Str& arg_, res::Props* props_);
    virtual void post_load(void);

    virtual const bool repos(const Vec3& pos_);
    virtual const bool resize(const Vec3& size_);
    virtual const bool rearrange(void);
    void set_size_strained(const bool size_strained_);
    void set_event_transparent(const bool event_transparent_);
    virtual const Vec3& get_bound(void);

    const bool is_event_transparent(void);

    virtual void enable(void);
    virtual void disable(void);
    virtual Widget* is_on(const Vec3& ptr_);
    virtual void pressed(const Vec3& pos_);
    virtual void released(const Vec3& pos_);
    virtual void activate(void);
    virtual void dragging(const Vec3& pos_);
    virtual void hover(void);
    virtual void typed(const InputCode key_);
    virtual void str(const Char* str_);

    virtual res::Props* gen_props(void);

    const res::Font* get_font(void) const;
    const Vec3& get_area(void) const;

    const bool update(const res::Material* bg_, const res::Font* fn_,
                      const Vec3& pad_);

protected:
    virtual const bool Adopt(Node* node_);
    void add_caption(const Str& txt_);
    res::Geometry* create_form(const Vec3& size_, const Vec3& pad_);

    virtual const Str& prop_bg(Node* arg_);
    const Str& prop_pad(const Str& arg_);
    const Str& prop_font(Node* arg_);
    const Str& prop_align(const Str& arg_);
    virtual const Str& prop_pos(const Str& arg_);
    virtual const Str& prop_size(const Str& arg_);
    const Str& prop_enable(const Str& arg_);
    virtual const Str& prop_src(Node* arg_);
    virtual const Str& prop_dst(Node* arg_);
    virtual const Str& prop_property(res::Property* arg_);

    bool enabled, size_strained, event_transparent;

    res::Model* form_mdl;
    res::Geometry* form_geom;
    const res::Font* font;

    res::Property* property;
    Str property_arg;

    Widget* youngest_child;
    Vec3 area, child_topleft, pad;
    Align align;
};
}
}
#endif
