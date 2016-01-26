#ifndef CANVAS_HPP_
#define CANVAS_HPP_

#include "Button.hpp"
#include "Caption.hpp"
#include "../Manipulator.hpp"

namespace reprize
{
namespace inf
{
class Canvas : public Button
{
public:
    Canvas(const Str& name_, const res::Material* bg_,
           const res::Font* fn_, const Vec3& pad_)
         : Button(name_, bg_, fn_, pad_), entity(NULL)
        {
            mtt = new vid::RenderState(new res::Model("Canvas_model"),
                                       Mtx44(), Vec3(1.0, 1.0, 1.0), 1.0, Mtx44(), true, 200,
                                       vid::SB_MATERIAL,
                                       vid::RT_COLOR | vid::RT_ZBUFFER,
                                       RS_CLEAR_Z, 0, RS_BF_SRC_ALPHA |
                                       (RS_BF_ONE_MINUS_SRC_ALPHA << 4));
        }
    Canvas(const Canvas& src_)
        : Button(src_), entity(NULL)
        {
            mtt = new vid::RenderState(new res::Model("Canvas_model"),
                                       Mtx44(), Vec3(1.0, 1.0, 1.0), 1.0, Mtx44(), true, 200,
                                       vid::SB_MATERIAL, vid::RT_COLOR | vid::RT_ZBUFFER,
                                       RS_CLEAR_Z, 0, RS_BF_SRC_ALPHA |
                                       (RS_BF_ONE_MINUS_SRC_ALPHA << 4));
        }
    virtual ~Canvas(void) {}

    virtual void eval(const res::Matter* parent_mtt_)
        {
            mtt->update();

            if (parent_mtt_->is_diff_mtx() || rel_mtx != pre_rel_mtx)
            {
                Mtx44 n_mtx(rel_mtx);
                n_mtx.mlt(parent_mtt_->get_n_mtx());
                mtt->set_n_mtx(n_mtx);
            }
            pre_rel_mtx = rel_mtx;

            // iterate children
            for (First(); !IsDone(); Next())
            {
                Entity* current = dynamic_cast<Entity*>(Current());
                if (current) { current->eval(mtt); }
            }
        }

    virtual Canvas* Clone(const bool with_child_ = true)
        {
            return new Canvas(*this);
        }
/*
    void init(Node* cmd_, const Str& prop_arg_, const Align align_ = TOP)
        {
            RE_DBG("canvas: %s", prop_arg_.c_str());
            if (dynamic_cast<Widget*>(cmd_))
            { RE_DBG("widget"); }
            else if (dynamic_cast<res::Entity*>(cmd_))
            {
                RE_DBG("entity");
                DEL(entity);
                // XXX
                entity = dynamic_cast<res::Entity*>(cmd)->Clone(true);
                Add(entity);
            }
            else if (dynamic_cast<res::Model*>(cmd_))
            {
                RE_DBG("model");
                res::Model* src_mdl = dynamic_cast<res::Model*>(cmd);
                DEL(entity);
                entity = new Entity("sample", new res::Matter(src_mdl));
                Add(entity);
            }
            else if (dynamic_cast<res::Material*>(cmd_))
            { RE_DBG("material"); }
            else if (dynamic_cast<res::Texture*>(cmd_))
            {
                RE_DBG("texture: ");
                res::Texture* found = dynamic_cast<res::Texture*>(cmd_);
                RE_DBG("%f, %f", found->get_width(), found->get_height());
                DEL(entity);
                // new Model("texture", geom, NULL);
                entity = new Entity("sample", new res::Matter(form_mdl));
                Add(entity);
            }
            else if (dynamic_cast<res::Geometry*>(cmd_)) { RE_DBG("geomety"); }
            // other view
            else if (dynamic_cast<vid::RenderState*>(cmd_)) { RE_DBG("rp"); }
            else if (cmd_) { RE_DBG("%s : node", cmd_->get_name().c_str()); }
            else { RE_DBG("canvas node is null"); }
        }
*/
    virtual const bool resize(const Vec3& size_)
        {
            Button::resize(size_);
            if (!entity) { return false; }

            const res::Model* src_mdl = entity->get_mtt()->get_model();
            if (!src_mdl) { return false; }
            Unit cs = src_mdl->get_geometry()->get_circumscription() * 2;
            Unit min = RE_MIN(size_.get_x(), size_.get_y());
            Unit scale = min / cs;
            entity->set_rel_scale(Vec3(scale, scale, scale));
            entity->set_rel_pos(area / 2);

            return true;
        }

    virtual void pressed(const Vec3& pos_) { Button::pressed(pos_); }
    virtual void released(const Vec3& pos_) { Button::released(pos_); }

    virtual void hover(void)  // need keyframe action? like 'flash'
        {
            /* RE_DBG("hovered"); */
        }

    virtual void activate(void)
        {
            RE_DBG("Activate");
        }

    virtual void dragging(const Vec3& pos_)
        {
            if (!entity) { return; }

            Vec3 offset(pos_ - prev_ptr_pos);

            if (area.get_x() && area.get_y())
            {
                Vec3 unit_v(1 / area.get_x(), 1 / area.get_y());
                offset *= unit_v * 180;
                entity->rotate(offset);
                entity->set_rel_pos(area / 2);
            }
            prev_ptr_pos = pos_;
        }

protected:
    Str content;
    bool updated;
    Entity* entity;
    Mtx44 mtx;
};
}
}

#endif

