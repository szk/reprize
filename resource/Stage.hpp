#ifndef STAGE_HPP_
#define STAGE_HPP_

#include "resource/Node.hpp"
#include "interface/Desktop.hpp"
#include "interface/HUD.hpp"

namespace reprize
{
namespace res
{
class Stage : public res::Node
{
public:
    Stage(const Str name_)
        : Node(name_), overlay(NULL), world(NULL), backdrop(NULL),
          menu(NULL), hud(NULL)
        {}
    virtual ~Stage(void) {}

    Props* gen_props(void)
        {
            RE_PROPS_START(Stage);
            RE_PROP_NODE(Stage, overlay);
            RE_PROP_NODE(Stage, world);
            RE_PROP_NODE(Stage, backdrop);
            RE_PROP_NODE(Stage, menu);
            RE_PROP_NODE(Stage, hud);
            return RE_PROPS_FINISH;
        }

    virtual Stage* Clone(const bool with_child_ = true)
        {
            Stage* stage = new Stage(*this);
            return stage;
        }
    res::Scene* get_overlay(void) { return overlay; }
    res::Scene* get_world(void) { return world; }
    res::Scene* get_backdrop(void) { return backdrop; }
    inf::Desktop* get_menu(void) { return menu; }
    inf::HUD* get_hud(void) { return hud; }

protected:
    const Str& prop_overlay(Node* arg_)
        {
            res::Scene* scene = dynamic_cast<res::Scene*>(arg_);
            if (NULL == scene) { return RE_FAILURE; }
            overlay = scene;
            RE_DBG("Overlay Prop: %s", scene->get_name().c_str());
            return RE_SUCCESS;
        }
    const Str& prop_world(Node* arg_)
        {
            res::Scene* scene = dynamic_cast<res::Scene*>(arg_);
            if (NULL == scene) { return RE_FAILURE; }
            world = scene;
            RE_DBG("World Prop: %s", scene->get_name().c_str());
            return RE_SUCCESS;
        }
    const Str& prop_backdrop(Node* arg_)
        {
            res::Scene* scene = dynamic_cast<res::Scene*>(arg_);
            if (NULL == scene) { return RE_FAILURE; }
            backdrop = scene;
            RE_DBG("Backdrop Prop: %s", scene->get_name().c_str());
            return RE_SUCCESS;
        }
    const Str& prop_menu(Node* arg_)
        {
            inf::Desktop* desktop = dynamic_cast<inf::Desktop*>(arg_);
            if (NULL == desktop) { return RE_FAILURE; }
            menu = desktop;
            RE_DBG("Menu Prop: %s", menu->get_name().c_str());
            return RE_SUCCESS;
        }
    const Str& prop_hud(Node* arg_)
        {
            inf::HUD* loaded_hud = dynamic_cast<inf::HUD*>(arg_);
            if (NULL == loaded_hud) { return RE_FAILURE; }
            hud = loaded_hud;
            RE_DBG("HUD Prop: %s", loaded_hud->get_name().c_str());
            return RE_SUCCESS;
        }

    res::Scene* overlay,* world,* backdrop;
    inf::Desktop* menu;
    inf::HUD* hud;
};
}
}
#endif
