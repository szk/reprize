#include "Common.hpp"

#include "input/Mouse.hpp"
#include "resource/Font.hpp"
#include "resource/Geometry.hpp"
#include "resource/Texture.hpp"
#include "resource/Model.hpp"
#include "Widget.hpp"

#include "Caption.hpp"

using namespace reprize;

using namespace res;
using namespace inf;
using namespace std;

Widget::Widget(const Str& name_, const Material* bg_, const Font* fn_,
               const Vec3& pad_, const Align align_)
    : Entity(name_), enabled(true), size_strained(false),
      event_transparent(false), form_mdl(NULL),
      form_geom(NULL), font(fn_), property(NULL), youngest_child(NULL),
      pad(pad_), align(align_)
{
    if (bg_ != NULL) { area.set(bg_->get_size()); }
    form_geom = create_form(area, pad_);
    form_mdl = new res::Model(name_, form_geom, bg_, BB_INDEPEND,
                              AU_TRANSPARENCY);
    mtt->set_model(form_mdl);
}

Widget::Widget(const Widget& src_)
    : Entity(src_), enabled(true), size_strained(src_.size_strained),
      event_transparent(false), form_mdl(NULL),
      form_geom(NULL), font(src_.font), property(NULL), youngest_child(NULL),
      area(src_.area), pad(src_.pad), align(src_.align)
{
    Vec3 bg_size(16, 16);
    if (NULL != src_.form_mdl && NULL != src_.form_mdl->get_material())
    { bg_size.set(src_.form_mdl->get_material()->get_size()); }

    form_geom = create_form(bg_size, pad);
    const Material* bg = NULL;
    if (NULL != src_.form_mdl) { bg = src_.form_mdl->get_material(); }
    form_mdl = new res::Model(src_.name, form_geom, bg, BB_INDEPEND,
                              AU_TRANSPARENCY);
    mtt->set_model(form_mdl);
}

Widget::~Widget(void)
{
}

void Widget::appendix(const Str& arg_, res::Props* props_)
{
    add_caption(arg_);
}

void Widget::post_load(void)
{
    rearrange();
}

const bool Widget::repos(const Vec3& pos_)
{
    set_rel_pos(pos_);
    return false;
}

const bool Widget::resize(const Vec3& size_)
{
    if (form_geom == NULL) { return false; }
    Vec3 form_size(16, 16);
    if (form_mdl->get_material() != NULL)
    { form_size.set(form_mdl->get_material()->get_size() - pad); }
    form_size.max(size_);
    area = form_size;

    for (Size32 i = 0; form_geom->get_vertex_n() > i; ++i)
    {
        if (i > 3 && i != 8 && i != 10)
        {
            if (i==4 || i==5 || i==12 || i==13 || i==20 || i==21)
            {
                form_geom->set_vertex_x(static_cast<Int32>(i),
                                        area.get_x() - pad.get_x());
            }
            else if (i==6 || i==7 || i==14 || i==15 || i==22 || i==23)
            { form_geom->set_vertex_x(static_cast<Int32>(i), area.get_x()); }

            if (i==9 || i==11 || i==13 || i==15 || i==16 || i==18 || i==20
                || i==22)
            {
                form_geom->set_vertex_y(static_cast<Int32>(i),
                                        area.get_y() - pad.get_y()); }
            else if (i==17 || i==19 || i==21 || i==23)
            { form_geom->set_vertex_y(static_cast<Int32>(i), area.get_y()); }
        }
    }

    return true;
}

const bool Widget::rearrange(void)
{
    Vec3 offset, child_area, enclosure;

    for (First(); !IsDone(); Next())
    {
        Widget* child = dynamic_cast<Widget*>(Current());
        if (!child) { continue; }
        child_area = child->get_area();

        // rearrange position
        switch (align)
        {
            // vertical align
        case MIDDLE:
            offset.set_y((area.get_y() - child_area.get_y() - pad.get_y() * 2)
                         / 2);
            break;
        case BOTTOM:
            offset.set_y(area.get_y() - child_area.get_y() - pad.get_y() * 2);
            break;
            // holizontal align
        case CENTER:
            if (area.get_x() >= child_area.get_x())
            {
                offset.set_x((area.get_x() - child_area.get_x() - pad.get_x()
                              * 2) / 2);
            }
            break;
        case RIGHT:
            if (area.get_x() >= child_area.get_x())
            {
                offset.set_x(area.get_x() - child_area.get_x() - pad.get_x()
                             * 2);
            }
            break;
        case ALIGN_NUM: break;
            // do nothing
        case LEFT: break;
        case TOP: break;
        }

        Vec3 pos(child->get_rel_pos());
        child->repos(pos + offset);
        // rearrange size
        Vec3 bottomright = child_area + pos + pad;
        enclosure.max(bottomright);
    }

    if (!size_strained) { area = enclosure; }
    else { area.max(enclosure); }

    resize(area);

    return true;
}

void Widget::set_size_strained(const bool size_strained_)
{
    size_strained = size_strained_;
}

void Widget::set_event_transparent(const bool event_transparent_)
{
    event_transparent = event_transparent_;
}

const Vec3& Widget::get_bound(void)
{
    return area;
}

const bool Widget::is_event_transparent(void)
{
    return event_transparent;
}

void Widget::enable(void)
{
    enabled = true;
}

void Widget::disable(void)
{
    enabled = false;
}

void Widget::pressed(const Vec3& pos_)
{
}

void Widget::released(const Vec3& pos_)
{
}

void Widget::activate(void)
{
    RE_DBG("Activate");
}

void Widget::dragging(const Vec3& pos_)
{
    RE_DBG("drag: %f, %f, %f", pos_.get_x(),pos_.get_y(),pos_.get_z());
}

void Widget::hover(void)
{
}

void Widget::typed(const InputCode key_)
{
}

void Widget::str(const Char* str_)
{
}

Widget* Widget::is_on(const Vec3& ptr_)
{
    Vec3 pos = mtt->get_c_mtx().get_pos();
    if (ptr_.get_x() < pos.get_x() || ptr_.get_y() < pos.get_y() ||
        ptr_.get_x() > (pos.get_x() + area.get_x()) ||
        ptr_.get_y() > (pos.get_y() + area.get_y()))
    { return NULL; }

    for (First(); !IsDone(); Next())
    {
        Widget* current_win = dynamic_cast<Widget*>(Current());
        if (current_win)
        {
            Widget* checked = current_win->is_on(ptr_);
            if (checked)
            {
                if (checked->is_event_transparent()) { return this; }
                return checked;
            }
        }
    }

    return this;
}

const bool Widget::Adopt(Node* node_)
{
    Widget* child = dynamic_cast<Widget*>(node_);
    if (child == NULL) { return Entity::Adopt(node_); }
    RE_DBG("-------put: %s", node_->get_name().c_str());

    if (youngest_child)
    {
        Vec3 stretch = youngest_child->get_area() + pad / 2;
        // child_topleft += strech;
        switch (align)
        {
        case TOP:
        case MIDDLE:
        case BOTTOM:
            child_topleft.add(stretch.get_x(), 0, 0);
            area.max(Vec3(child_topleft.get_x() + child->get_area().get_x()
                          + pad.get_x() * 2));
            break;
        case LEFT:
        case CENTER:
        case RIGHT:
            child_topleft.add(0, stretch.get_y(), 0);
            area.max(Vec3(0,
                          child_topleft.get_y() + child->get_area().get_y()
                          + pad.get_y() * 2));
            break;
        case ALIGN_NUM: break;
        }
    }
    else
    {
        child_topleft = pad;
        Vec3 new_area = child->get_area() + pad * 2;
        area.max(new_area);
    }

    area.max(child_topleft + child->get_area() + pad);

    resize(area);
    child->repos(child_topleft);
    youngest_child = child;

    return Entity::Adopt(child);
}


Props* Widget::gen_props(void)
{
    RE_PROPS_START(Widget);
    RE_PROP_NODE(Widget, bg);
    RE_PROP_STR(Widget, pad);
    RE_PROP_NODE(Widget, font);
    RE_PROP_STR(Widget, align);
    RE_PROP_STR(Widget, pos);
    RE_PROP_STR(Widget, size);
    RE_PROP_STR(Widget, enable);
    RE_PROP_NODE(Widget, src);
    RE_PROP_NODE(Widget, dst);
    RE_PROP_PROP(Widget, property);

    return RE_PROPS_FINISH;
}

Widget* Widget::Clone(const bool with_child_)
{
    return new Widget(*this);
}

const res::Font* Widget::get_font(void) const
{
    return font;
}

const Vec3& Widget::get_area(void) const
{
    return area;
}

const bool Widget::update(const res::Material* bg_, const res::Font* fn_,
                          const Vec3& pad_)
{
    if (bg_)
    {
        area.set(bg_->get_size());
        DEL(form_geom);
        pad = pad_;
        form_geom = create_form(area, pad);

        DEL(form_mdl);
        form_mdl = new res::Model("Widget", form_geom, bg_, BB_INDEPEND,
                                  AU_TRANSPARENCY);
        mtt->set_model(form_mdl);
    }

    if (fn_) { font = fn_; }

    return bg_ && fn_;
}

void Widget::add_caption(const Str& txt_)
{
    if (txt_.empty()) { return; }
    Caption* caption = new Caption("Caption");
    Add(caption);
    if (caption) { caption->setf(font, 0, txt_.c_str()); }
    RE_DBG("caption added: %s", txt_.c_str());
}

Geometry* Widget::create_form(const Vec3& size_, const Vec3& pad_)
{
    Geometry* geometry = new Geometry("Form");
    Unit x = size_.get_x();
    Unit y = size_.get_y();
    Unit x_remain = x - pad_.get_x();
    Unit y_remain = y - pad_.get_y();

    Unit u_left = pad_.get_x() / x, v_top = pad_.get_y() / y,
        u_right = x_remain / x, v_bottom = y_remain / y;

    // upper
    geometry->add_mesh(TRIANGLE_STRIP);
    for (Size32 i = 0; 8 > i; ++i) { geometry->add_mesh_idx(i); }
    geometry->add_vertex(0,            0,            0, 0,       0);
    geometry->add_vertex(0,            pad_.get_y(), 0, 0,       v_top);
    geometry->add_vertex(pad_.get_x(), 0,            0, u_left,  0);
    geometry->add_vertex(pad_.get_x(), pad_.get_y(), 0, u_left,  v_top);
    geometry->add_vertex(x_remain,     0,            0, u_right, 0);
    geometry->add_vertex(x_remain,     pad_.get_y(), 0, u_right, v_top);
    geometry->add_vertex(x,            0,            0, 1,       0);
    geometry->add_vertex(x,            pad_.get_y(), 0, 1,       v_top);

    // middle
    geometry->add_mesh(TRIANGLE_STRIP);
    for (Size32 i = 8; 16 > i; ++i) { geometry->add_mesh_idx(i); }
    geometry->add_vertex(0,            pad_.get_y(), 0, 0,       v_top);
    geometry->add_vertex(0,            y_remain,     0, 0,       v_bottom);
    geometry->add_vertex(pad_.get_x(), pad_.get_y(), 0, u_left,  v_top);
    geometry->add_vertex(pad_.get_x(), y_remain,     0, u_left,  v_bottom);
    geometry->add_vertex(x_remain,     pad_.get_y(), 0, u_right, v_top);
    geometry->add_vertex(x_remain,     y_remain,     0, u_right, v_bottom);
    geometry->add_vertex(x,            pad_.get_y(), 0, 1,       v_top);
    geometry->add_vertex(x,            y_remain,     0, 1,       v_bottom);

    // lower
    geometry->add_mesh(TRIANGLE_STRIP);
    for (Size32 i = 16; 24 > i; ++i) { geometry->add_mesh_idx(i); }
    geometry->add_vertex(0,            y_remain, 0, 0,       v_bottom);
    geometry->add_vertex(0,            y,        0, 0,       1);
    geometry->add_vertex(pad_.get_x(), y_remain, 0, u_left,  v_bottom);
    geometry->add_vertex(pad_.get_x(), y,        0, u_left,  1);
    geometry->add_vertex(x_remain,     y_remain, 0, u_right, v_bottom);
    geometry->add_vertex(x_remain,     y,        0, u_right, 1);
    geometry->add_vertex(x,            y_remain, 0, 1,       v_bottom);
    geometry->add_vertex(x,            y,        0, 1,       1);

    return geometry;
}

const Str& Widget::prop_bg(Node* arg_)
{
    if (!dynamic_cast<Material*>(arg_)) { return RE_FAILURE; }
    form_mdl->Add(dynamic_cast<Material*>(arg_));
    return RE_SUCCESS;
}

const Str& Widget::prop_pad(const Str& arg_)
{
    pad.set(arg_);
    RE_DBG("%f, %f, %f", pad.get_x(), pad.get_y(), pad.get_z());
    return RE_SUCCESS;
}

const Str& Widget::prop_font(Node* arg_)
{
    if (!dynamic_cast<Font*>(arg_)) { return RE_FAILURE; }
    font = dynamic_cast<Font*>(arg_);
    return RE_SUCCESS;
}

const Str& Widget::prop_align(const Str& arg_)
{
    align = LEFT;
    if (arg_ == "top") { align = TOP; }
    else if (arg_ == "middle") { align = MIDDLE; }
    else if (arg_ == "bottom") { align = BOTTOM; }
    else if (arg_ == "left")   { align = LEFT; }
    else if (arg_ == "center") { align = CENTER; }
    else if (arg_ == "right")  { align = RIGHT; }
    return RE_SUCCESS;
}

const Str& Widget::prop_pos(const Str& arg_)
{
    repos(Vec3(arg_));
    return RE_SUCCESS;
}

const Str& Widget::prop_size(const Str& arg_)
{
    area.set(arg_);
    size_strained = true;
    resize(area);

    return RE_SUCCESS;
}

const Str& Widget::prop_enable(const Str& arg_)
{
    return RE_SUCCESS;
}

const Str& Widget::prop_src(Node* arg_)
{
    return RE_SUCCESS;
}

const Str& Widget::prop_dst(Node* arg_)
{
    return RE_SUCCESS;
}

const Str& Widget::prop_property(Property* arg_)
{
    property = arg_;
    return RE_SUCCESS;
}
