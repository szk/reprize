#include "Common.hpp"

#include "loader/GeomLoader.hpp"
#include "loader/TexLoader.hpp"
#include "loader/FontLoader.hpp"
#include "Actor.hpp"
#include "Model.hpp"
#include "Geometry.hpp"

#include "interface/Theme.hpp"
#include "interface/Desktop.hpp"

#include "Body.hpp"
#include "physics/Ray.hpp"
#include "physics/Tris.hpp"
#include "physics/Sphere.hpp"
#include "physics/SphereTree.hpp"

#include "scene/Space.hpp"

#include "Text.hpp"

using namespace reprize;
using namespace res;
using namespace inf;
using namespace vid;
using namespace std;

Builder::Builder(VFS* vfs_)
{
    pvt = new bld_pvt;
    pvt->vfs = vfs_;
}

Builder::~Builder(void)
{
}

const bool Builder::add_loader(Loader* loader_)
{
    if (dynamic_cast<GeomLoader*>(loader_))
    { pvt->geom_loader = dynamic_cast<GeomLoader*>(loader_); }
    else if (dynamic_cast<TexLoader*>(loader_))
    { pvt->tex_loader = dynamic_cast<TexLoader*>(loader_); }
    else if (dynamic_cast<FontLoader*>(loader_))
    { pvt->font_loader = dynamic_cast<FontLoader*>(loader_); }
    else
    {
        if (loader_)
        {
            cerr << "unsupported loader by builder : "
                 << loader_->get_name() << endl;
        }
        return false;
    }

    return true;
}

const bool Builder::set_parent(Node* node_)
{
    pvt->parent = node_;
    return pvt->parent != NULL;
}

const bool Builder::set_parent(const Str& name_)
{
    pvt->parent = pvt->parent->find(name_);
    return pvt->parent != NULL;
}

const Str& Builder::reference(const Str& path_)
{
    if (!pvt->parent) { return path_; }
    pvt->parent->SysAdd(pvt->vfs->find(path_));
    return path_;
}

void Builder::set_type_name(const Str& name_)
{
    pvt->type_name = name_;
}

const Str& Builder::get_type_name(void)
{
    return pvt->type_name;
}

const Str& Builder::material(const Str& name_)
{
    Material* result = new Material(name_);
    pvt->parent->SysAdd(result);
    return result->get_name();
}

// const Str& Builder::actor(const Str& path_)
// {
//     if (!pvt->parent) { return path_; }
//     g_log->printf("actor registered: %s", path_.c_str());

//     Actor* result = dynamic_cast<Actor*>(pvt->vfs->find(path_));
//     if (!result) { return path_; }
//     pvt->parent->SysAdd(result);
//     if (dynamic_cast<Actor*>(pvt->parent))
//     { dynamic_cast<Actor*>(pvt->parent)->Add(result); }

//     return result->get_name();
// }

const Str& Builder::actor(const Str& name_,
                          const Unit x, const Unit y, const Unit z,
                          const Unit w, const Unit h, const Unit d)
{
    if (!pvt->parent) { return name_; }
    g_log->printf("actor generating: %s", name_.c_str());
    Actor* result = new Actor(name_, NULL);
    result->set_rel_pos(Vec3(x, y, z));
    result->set_rel_scale(Vec3(w, h, d));
    pvt->parent->SysAdd(result);

    if (dynamic_cast<Actor*>(pvt->parent))
    {
        dynamic_cast<Actor*>(pvt->parent)->Add(result);
        g_log->printf("rs registered: %s", result->get_name());
    }

    return result->get_name();
}

const Str& Builder::model(const Str& name_, const Str& billboard_,
                          const Str& alpha_)
{
    BillBoard bb = BB_INDEPEND;
    if (billboard_ == "screen") { bb = BB_SCR; }
    else if (billboard_ == "x") { bb = BB_X; }
    else if (billboard_ == "y") { bb = BB_Y; }
    else if (billboard_ == "z") { bb = BB_Z; }

    AlphaUsage a_usage = AU_TRANSPARENCY; // expected
    if (alpha_ == "trn") { a_usage = AU_TRANSPARENCY; }
    else if (alpha_ == "ref") { a_usage = AU_REFLECTION; }

    g_log->printf("try to add model: %s", name_.c_str());
    if (pvt->parent == NULL) { return name_; }

    Model* result = new Model(name_, NULL, NULL, bb, a_usage);
    pvt->parent->SysAdd(result);

    return result->get_name();
}

const Str& Builder::theme(const Str& name_)
{
    Theme* theme = dynamic_cast<Theme*>(pvt->vfs->find(name_));
    Material* mtl = dynamic_cast<Material*>
        (pvt->vfs->find("/System/Material/Default"));
    Font* fn = dynamic_cast<Font*>(pvt->vfs->find("/System/Font/ReFont"));
    Font* mi_fn = dynamic_cast<Font*>(pvt->vfs->find("/System/Font/MicroFont"));

//     Str p_snd_path = xml_->get_att("pressed_sound");
//     Str r_snd_path = xml_->get_att("released_sound");
//     Str d_snd_path = xml_->get_att("dragging_sound");

//     Sound* pre_s = dynamic_cast<Sound*>(vfs->find(p_snd_path));
//     Sound* rel_s = dynamic_cast<Sound*>(vfs->find(r_snd_path));
//     Sound* dra_s = dynamic_cast<Sound*>(vfs->find(d_snd_path));

//     if (dynamic_cast<Sound*>(pre_s))
//     { cerr << "p_snd_path is fine: " << pre_s->get_name() << endl; }
//     else { cerr << "p_snd_path is not fine" << endl; }

    if (!theme && pvt->parent)
    {
        theme = new Theme(name_, mtl, fn, mi_fn, NULL, NULL, NULL);
        pvt->parent->SysAdd(theme);
    }
    return name_;
}

const Str& Builder::theme(const Str& type_, const Str& txt_, const StrMap& am_)
{
    Material* mtl = NULL;
    if (!(mtl = dynamic_cast<Material*>(pvt->vfs->find(txt_))))
    {
        g_log->printf("Theme:material: %s is not found.", txt_.c_str());
        return type_;
    }
    else { g_log->printf("Theme:material: %s", txt_.c_str()); }

    Font* font = NULL;
    Str fn_path = ATT(am_, "font");
    if (!(font = dynamic_cast<Font*>(pvt->vfs->find(fn_path))))
    {
        g_log->printf("Theme:font: %s is not found.", fn_path.c_str());
        return type_;
    }
    else { g_log->printf("Theme:font: %s", fn_path.c_str()); }

    stringstream ss(ATT(am_, "vpad"));
    Int32 vpad; ss >> vpad;
    if (vpad > 320) { vpad = 4; }
    stringstream sh(ATT(am_, "hpad"));
    Int32 hpad; sh >> hpad;
    if (hpad > 320) { hpad = 4; }
    if (dynamic_cast<Theme*>(pvt->parent))
    {
        dynamic_cast<Theme*>(pvt->parent)->
            set_appearance(type_, mtl, font, vpad, hpad);
    }
    return type_;
}

const Str& Builder::desktop(const Str& name_)
{
    // FIXME: consider to use for the 'in scene' desktop
    Desktop* desktop = new Desktop(name_,Vec3(SCREEN_WIDTH, SCREEN_HEIGHT),
                                   dynamic_cast<Material*>
                                   (pvt->vfs->find("/System/Material/Default")),
                                   dynamic_cast<Font*>
                                   (pvt->vfs->find("/System/Font/ReFont")));
    pvt->parent->SysAdd(desktop);
    return name_;
}

// for menu
const Str& Builder::desktop(const Str& name_, const Str& theme_)
{
    Theme* theme = NULL;
    if (!(theme = dynamic_cast<Theme*>(pvt->vfs->find(theme_))))
    { theme = dynamic_cast<Theme*>(pvt->vfs->find("/System/Theme")); }
    Desktop* desktop = dynamic_cast<Desktop*>(pvt->parent);
    if (desktop)
    {
        desktop->set_theme(theme);
    }
//     return desktop->get_name();
    return name_;
}

const Str& Builder::desktop(const Str& type_, const Str& txt_, const StrMap& am_)
{
    Str cmd_str(ATT(am_, "cmd")), type_str(type_);
    Str caption(txt_);

    if (!caption.empty()) { type_str = "Caption"; }

    // check align
    Align align = LEFT;
    if (ATT(am_, "align") == "top") { align = TOP; }
    else if (ATT(am_, "align") == "middle") { align = MIDDLE; }
    else if (ATT(am_, "align") == "bottom") { align = BOTTOM; }
    else if (ATT(am_, "align") == "left") { align = LEFT; }
    else if (ATT(am_, "align") == "center") { align = CENTER; }
    else if (ATT(am_, "align") == "right") { align = RIGHT; }

    // get size
    Vec3 size(2, 2);
    bool strained = false;
    if (!ATT(am_, "size").empty())
    {
        size.set(ATT(am_, "size"));
        strained = true;
    }

    Size32 cmd_begin = cmd_str.find_first_not_of(CMD_SEPARATOR);
    Size32 cmd_len = cmd_str.find(CMD_SEPARATOR, cmd_begin);

    Str cmd_tok(cmd_str), arg_tok;

    if (cmd_len != Str::npos)
    {
        cmd_tok = cmd_str.substr(cmd_begin, cmd_len - cmd_begin);
        Size32 arg_pos = cmd_str.find_first_not_of(CMD_SEPARATOR, cmd_len);
        if (arg_pos != Str::npos)
        { arg_tok = cmd_str.substr(arg_pos, cmd_str.length()); }
    }

    Node* cmd_node = NULL;
    if (!(cmd_node = pvt->vfs->find(cmd_tok)) && !cmd_str.empty())
    { g_log->printf("Command: %s is not found", cmd_str.c_str()); }

    bool joint = false; // XXX
//     if (dynamic_cast<Desktop*>(pvt->parent)) { joint = true; }

    Theme* theme = dynamic_cast<Theme*>(pvt->vfs->find("/System/Theme"));
    Window* body = theme->copy_widget(
        dynamic_cast<Window*>(pvt->parent), type_str, joint,
        caption, cmd_node,
        arg_tok, size, strained, align);

//    Check it in VarLoader
    dynamic_cast<Window*>(pvt->parent)->rearrange();

    return type_;
}

const Str& Builder::texture(const Str& path_)
{
    g_log->printf("Texture applyed: %s", path_.c_str());
    Material* mtl = dynamic_cast<Material*>(pvt->parent);
    Texture* result = dynamic_cast<Texture*>(pvt->vfs->find(path_));
    if (mtl && result) { mtl->add_texture(result); }
    return result->get_name();
}

const Str& Builder::texture(const Str& name_, const uInt32* array_,
                            const uInt32 w_pow_, const TextureFiltering tf_)
{
    if (pvt->tex_loader == NULL) { return pvt->def_tex; }
    Texture* result = NULL;
    switch (tf_)  // XXX NOT IMPLEMENTED
    {
    case BILINEAR:
    case TRILINEAR:
    case ANISOTROPIC:
        result = pvt->tex_loader->build(name_, array_, w_pow_);
        break;
    }
    pvt->parent->SysAdd(result);
    return result->get_name();
}

const Str& Builder::text(const Str& name_, const Str& content_,
                         const Str& font_)
{
    Text* result = new Text(name_);
    Font* fn = dynamic_cast<Font*>(pvt->vfs->find(font_));
    result->set(content_, fn);
    pvt->parent->SysAdd(result);
    return result->get_name();
}

const Str& Builder::geometry(const Str& name_, const Size32** mesh_,
                             const Unit* vertex_, const Unit* uv_)
{
    if (!pvt->parent) { return pvt->def_geom; }
    Geometry* result = new Geometry(name_);

    for (Size32 n = 0; vertex_[n*3 + 1] && uv_[n*2 + 1]; ++n)
    {
        result->add_vertex(vertex_[n*3], vertex_[n*3 + 1],vertex_[n*3 + 2],
                           uv_[n*2], uv_[n*2 + 1]);
    }

//     return pvt->root->get_name() + "/" + pvt->geom->get_name() + "/" + name_;
//     std::cerr << "uv/vert is completed";

    if (!mesh_) { return pvt->def_geom; }

    for (Size32 m = 0; mesh_[m]; ++m)
    {
        result->add_mesh(TRIANGLE_STRIP);
//         for (Size32 m_id = 0; mesh_[m][m_id]; ++m_id)
//         { result->add_mesh_idx(mesh_[m][m_id]); }
    }

    pvt->parent->SysAdd(result);

    return result->get_name();
}

const Str& Builder::geometry(Geometry* src_)
{
    if (!pvt->parent) { return src_->get_name(); }
    pvt->parent->SysAdd(src_);
    return src_->get_name();
}

const Str& Builder::body(const Str& type_,
                         const Str& friction_, const Str& mass_)
{
    Body* result = NULL;
    if (type_ == "Ray") { result = new phy::Ray(); }
    else if (type_ == "Tris")
    { result = new phy::Tris(Vec3(1, 1, 1), Vec3(1, 1, 1), Vec3(1, 1, 1)); }
    else if (type_ == "Sphere") { result = new phy::Sphere(3); }
    else if (type_ == "SphereTree")
    { Vec3 tmp_v(1, 1, 1); result = new phy::SphereTree(tmp_v, tmp_v, tmp_v); }

    pvt->parent->SysAdd(result);
    return result->get_name();
}

const Str& Builder::sound(const Str& name_)
{
    return pvt->def_snd;
}

const Str& Builder::space(const Str& name_, const Str& type_)
{
    scn::Space* space = NULL;
    return pvt->def_spc;
}
