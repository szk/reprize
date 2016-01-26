#ifndef SPEAKER_HPP_
#define SPEAKER_HPP_

namespace reprize
{
namespace res { class Sound; }

namespace aud
{
typedef res::PropStr<Speaker> SPPropStr;

class Speaker
{
public:
    Speaker(void)
        : def_snd(NULL), frame_d(0), played_sound(0), dev(true)
        { node = new res::Node("Speaker"); }
    virtual ~Speaker(void) {}

    res::Node* get_node(void) { return node; }
    virtual const bool init(void)
        {
//             node->Put(new res::Node("+show_tris",
//                                     new SRPropStr(this,
//                                               &Speaker::cmd_on_showtris)));
//             node->Put(new res::Node("-show_tris",
//                                     new SRPropStr(this,
//                                              &Speaker::cmd_off_showtris)));
//             node->Put(new res::Node("+show_pts",
//                                     new SRPropStr(this,
//                                               &Speaker::cmd_on_showpts)));
//             node->Put(new res::Node("-show_pts",
//                                     new SRPropStr(this,
//                                               &Speaker::cmd_off_showpts)));
//             def_geometry = def_geom_;
//             def_material = new res::Material("Default_mtl");
//             def_material->add_visual(def_vis_);
            return true;
        }
//     virtual void set_frame_d(Ratio d_) { frame_d = d_; }
//     virtual void render(vid::RenderState* rs_) {}

//     Unit cmd_on_showtris(const Str& arg_) { show_tris = true; return 0; }
//     Unit cmd_off_showtris(const Str& arg_) { show_tris = false; return 0; }
//     Unit cmd_on_showpts(const Str& arg_) { show_pts = true; return 0; }
//     Unit cmd_off_showpts(const Str& arg_) { show_pts = false; return 0; }

protected:
//     virtual void set_fov(Float64 fovy_, Float64 aspect_,
//                          Float64 zNear_, Float64 zFar_) {}
//     virtual void draw(vid::RenderState* rs_) {}

    res::Node* node;
    res::Sound* def_snd;
//     res::Geometry* def_geometry;

    Ratio frame_d;
//     Mtx44 overlay_mtx, backdrop_mtx;

    uInt32 played_sound;
    bool dev;
};
}
}
#endif
