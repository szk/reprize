#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "Node.hpp"

#include "Geometry.hpp"
#include "Material.hpp"
#include "Shader.hpp"

namespace reprize
{
enum BillBoard { BB_INDEPEND, BB_X, BB_Y, BB_Z, BB_SCR, BB_NUM, };
enum AlphaUsage { AU_IGNORE, AU_TRANSPARENCY, AU_REFLECTION, AU_NUM, };

namespace res
{
class Model : public Node
{
public:
    Model(Str name_, const Geometry* geom_ = NULL, const Material* mtl_ = NULL,
          const BillBoard view_ = BB_INDEPEND,
          const AlphaUsage alpha_ = AU_IGNORE)
        : Node(name_), geometry(geom_), material(mtl_), view(view_),
          alpha(alpha_)
        {
        }
    Model(const Model& src_)
        : Node(src_.name), geometry(src_.geometry), material(src_.material),
          view(src_.view), alpha(src_.alpha)
        {
        }
    virtual ~Model(void) {}

    virtual Model* Clone(const bool with_child_ = true)
        { return new Model(*this); }

    virtual Props* gen_props(void)
        {
            RE_PROPS_START(Model);
            RE_PROP_NODE(Model, geometry);
            RE_PROP_NODE(Model, material);
            RE_PROP_STR(Model, view);
            RE_PROP_STR(Model, alpha);
            return RE_PROPS_FINISH;
        }

    inline const Geometry* get_geometry(void) const { return geometry; }
    inline const Material* get_material(void) const { return material; }
    inline const BillBoard get_billboard(void) const { return view; }

    inline const bool is_transparent(void) const
        {
            if (material)
            { return material->has_alpha() && alpha == AU_TRANSPARENCY; }
            return false;
        }

protected:
    const Str& prop_geometry(Node* arg_)
        {
            if (dynamic_cast<Geometry*>(arg_))
            {
                RE_DBG("geometry: %s", arg_->get_name().c_str());
                geometry = dynamic_cast<Geometry*>(arg_);
            }
            return RE_SUCCESS;
        }
    const Str& prop_material(Node* arg_)
        {
            if (dynamic_cast<Material*>(arg_))
            {
                RE_DBG("material: %s", arg_->get_name().c_str());
                material = dynamic_cast<Material*>(arg_);
            }
            return RE_SUCCESS;
        }
    const Str& prop_view(const Str& arg_)
        { RE_DBG("%d", view); return RE_SUCCESS; }

    const Str& prop_alpha(const Str& arg_)
        {
            switch (alpha)
            {
            case AU_REFLECTION:
            case AU_TRANSPARENCY:
            case AU_IGNORE:
            default: break;
            }
            RE_DBG("alphausage");
            return RE_SUCCESS;
        }

    const Geometry* geometry;
    const Material* material;
    const BillBoard view;
    const AlphaUsage alpha;

private:
    // XXX prepare NULL Node that delete slot node.
    virtual const bool Adopt(Node* node_)
        {
            if (dynamic_cast<Geometry*>(node_))
            {
                RE_DBG("geometry: %s", node_->get_name().c_str());
                geometry = dynamic_cast<Geometry*>(node_);
            }
            else if (dynamic_cast<Material*>(node_))
            {
                RE_DBG("material: %s", node_->get_name().c_str());
                material = dynamic_cast<Material*>(node_);
            }
            return Node::Adopt(node_);
        }
};
}
}
#endif
