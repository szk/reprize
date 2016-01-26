#ifndef DIV_HPP_
#define DIV_HPP_

#include "Widget.hpp"
#include "Caption.hpp"

namespace reprize
{
namespace inf
{
class Div : public Widget
{
public:
    Div(const Str& name_, const res::Material* bg_,
        const res::Font* fn_, const Vec3& pad_)
        : Widget(name_, bg_, fn_, pad_), src_entity(NULL), src_property(NULL),
          tgt(NULL)
        {}
    Div(const Div& src_)
        : Widget(src_), src_entity(NULL), src_property(NULL), tgt(NULL)
        {}
    virtual ~Div(void) {}

    virtual void eval(const res::Matter* parent_mtt_)
        {
            if (src_entity)
            {
                if (tgt != NULL)
                {
                    Vec3 position = src_entity->get_mtt()->get_c_mtx().get_pos();
                    tgt->setf(font, tgt->get_area().get_x(),
                              position.get_str().c_str());
//                     RE_DBG("%s: %s", src_entity->get_name().c_str(),
//                            position.get_str().c_str());
                }
            }
            return Entity::eval(parent_mtt_);
        }

    virtual Div* Clone(const bool with_child_ = true)
        {
            return new Div(*this);
        }

protected:
    virtual const Str& prop_src(res::Node* arg_)
        {
            RE_DBG("prop_src called");
            src_entity = dynamic_cast<res::Entity*>(arg_);
            if (src_entity)
            {
                RE_DBG("prop_src");
                tgt = dynamic_cast<Caption*>(entry->Find(Str("Caption")));
                if (tgt)
                {
                    RE_DBG("prop tgt");
                }
            }
            return RE_SUCCESS;
        }
    virtual const Str& prop_property(res::Property* arg_)
        {
            src_property = dynamic_cast<res::Property*>(arg_);
            return RE_SUCCESS;
        }

    res::Entity* src_entity;
    res::Property* src_property;
    Caption* tgt;
};
}
}
#endif
