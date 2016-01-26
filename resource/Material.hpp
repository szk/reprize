#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "Node.hpp"
#include "TexAtlas.hpp"
#include "Texture.hpp"

namespace reprize
{
namespace res
{
class Material : public Node
{
public:
    Material(const Str name_)
        : Node(name_, NULL), alpha(false), fallback_tex(NULL)
        {
        }
    Material(const Material& src_)
        : Node(src_.name, NULL), alpha(src_.alpha),
          fallback_tex(src_.fallback_tex)
        {
        }
    virtual ~Material(void) {}

    virtual Material* Clone(const bool with_child_ = true)
        {
            Material* mtl = new Material(*this);
            entry->CloneChild(mtl);
            return mtl;
        }

    const Size32 get_tex_n(void) const { return texture.size(); }
    const Texture* get_tex(Size32 n_) const
        {
            if (n_ < texture.size()) { return texture.at(n_); }
            return NULL;
        }

    const Vec3& get_size(void) const { return size; }

//     Int32 get_width(void) const
//         {
//             if (fallback_tex == NULL) { return 8; }
//             return fallback_tex->get_width();
//         }
//     Int32 get_height(void) const
//         {
//             if (fallback_tex == NULL) { return 8; }
//             return fallback_tex->get_height();
//         }
    const bool has_alpha(void) const { return alpha; }
    virtual const bool add_texture(const Texture* tex_)
        {
            if (tex_)
            {
                fallback_tex = tex_;
                texture.push_back(tex_);
                size.max(Vec3(static_cast<Unit>(tex_->get_width()),
                              static_cast<Unit>(tex_->get_height()), 0));
                /*
                switch (tex_->get_type())
                {
                case res::VIS_TEXTURE: texture.push_back(tex_); break;
                case res::VIS_SHADER: shader.push_back(tex_); break;
                default: break;
                }
                */
            }

            return false;
        }

protected:
    virtual const bool Adopt(Node* node_)
        {
            const Texture* income = dynamic_cast<const Texture*>(node_);
            if (!income) { return Node::Adopt(node_); }
            add_texture(income);

            return Node::Adopt(node_);
        }

    bool alpha;
    Vec3 size;
    const Texture* fallback_tex;
    std::vector<const Texture*>texture, shader;
};
}
}

#endif
