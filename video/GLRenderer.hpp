#ifndef GL_RENDERER_HPP_
#define GL_RENDERER_HPP_

#include "Renderer.hpp"
#include "resource/Mesh.hpp" // for PrimMode

#include "resource/Matter.hpp"
#include "RenderState.hpp"

namespace reprize
{
namespace res { class Geometry; class Material; class Matter; class Texture; }

namespace vid
{
class GLRenderer : public Renderer
{
public:
    GLRenderer(void);
    virtual ~GLRenderer(void);

    virtual void init(uInt32 width_, uInt32 height_, res::Geometry* def_geom_,
                      res::Texture* def_tex_);
    virtual void render(vid::RenderState* rp_);

protected:
    void bind_material(const res::Material* mtl_);
    void apply_state(RenderState* rs_);
    virtual GLenum get_gl_prim(const res::PrmMode mode_);
    virtual GLuint get_gl_bf(const uInt32 factor_);

    void set_fov(Unit fov_y_, Unit aspect_, Unit z_near_, Unit z_far_);
    void draw(vid::RenderState* rs_);

    Unit aspect;
    NID current_mtl_nid;
};
}
}
#endif
