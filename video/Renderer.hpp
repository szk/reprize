#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include "resource/Material.hpp"

namespace reprize
{
namespace res
{
class Texture;
class Mesh;
class Geometry;
}

namespace vid
{
typedef res::PropStr<Renderer> RndrPropStr;

class Renderer
{
public:
    Renderer(void)
        : def_material(NULL), def_geometry(NULL), frame_d(0), drawn_matter(0),
          dev(true), show_tris(false), show_pts(false)
        { node = new res::Node("Renderer"); }
    virtual ~Renderer(void) {}

    res::Node* get_node(void) { return node; }
    virtual void init(uInt32 width_, uInt32 height_, res::Geometry* def_geom_,
                      res::Texture* def_tex_)
        {
            res::Props* pr = new res::Props;
            pr->Add("show_tris",
                    new RndrPropStr(this, &Renderer::prop_showtris));
            pr->Add("show_pts",
                    new RndrPropStr(this, &Renderer::prop_showpts));

            def_geometry = def_geom_;
            def_material = new res::Material("Default_mtl");
            def_material->add_texture(def_tex_);
        }
    virtual void set_frame_d(Ratio d_) { frame_d = d_; }
    virtual void render(vid::RenderState* rs_) {}

protected:
    const Str& prop_showtris(const Str& arg_)
        { show_tris = true; return RE_SUCCESS; }
    const Str& prop_showpts(const Str& arg_)
        { show_pts = true; return RE_SUCCESS; }

    virtual void set_fov(Unit fovy_, Unit aspect_, Unit zNear_, Unit zFar_) {}
    virtual void draw(vid::RenderState* rs_) {}

    res::Props pr;
    res::Node* node;
    res::Material* def_material;
    res::Geometry* def_geometry;

    Ratio frame_d;
    Mtx44 overlay_mtx, backdrop_mtx;

    uInt32 drawn_matter;
    bool dev, show_tris, show_pts;
};
}
}
#endif
