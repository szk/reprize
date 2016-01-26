#include "Common.hpp"

#include "input/Mouse.hpp"

#include "loader/VarLoader.hpp"
#include "loader/ScrptLoader.hpp"
#include "loader/GeomLoader.hpp"

#include "reader/FSReader.hpp"
#include "reader/zlibReader.hpp"
#include "reader/NetReader.hpp"

#include "Node.hpp"
#include "Model.hpp"

#include "Keyframe.hpp"

#include "Entity.hpp"
#include "Body.hpp"
#include "Actor.hpp"
#include "ParticleHolder.hpp"
#include "Text.hpp"
#include "Brush.hpp"

#include "interface/Theme.hpp"
#include "interface/Desktop.hpp"
#include "interface/HUD.hpp"
#include "scene/Scene.hpp"
#include "scene/Space.hpp"
#include "Stage.hpp"

#include "Generator.hpp"

#include "physics/Sphere.hpp"
#include "physics/Ray.hpp"
#include "physics/Tris.hpp"
#include "physics/SphereTree.hpp"

#include "default_var.hpp"
#include "default_res.hpp"

using namespace std;

reprize::mem::MemHunk reprize::memhunk;

using namespace reprize;
using namespace phy;
using namespace res;
using namespace inf;

Resource::Resource(void)
    : node(NULL)
{
    pvt = new res_pvt;
    pvt->vfs = new VFS;
    ///
    pvt->define_root = new Node("Define");
    pvt->vfs->add_to_root(pvt->define_root);
    ///

    g_log = new Paragraph("Global");
    node = new Node("Resource");
}

Resource::~Resource(void)
{
    DEL(pvt->vfs);

    for (list<Reader*>::iterator itr = pvt->reader_l.begin();
         itr != pvt->reader_l.end(); ++itr)
    { DEL((*itr)); }

    DEL(pvt);
    DEL(node);
}

res::Node* Resource::get_node(void)
{
    return node;
}

void Resource::add_reader(Reader* reader_)
{
    if (reader_) { pvt->reader_l.push_back(reader_); }
}

void Resource::add_loader(Loader* loader_)
{
    if (loader_ == NULL) { return; }
    for (list<Reader*>::iterator itr = pvt->reader_l.begin();
         itr != pvt->reader_l.end(); ++itr)
    { (*itr)->add_loader(loader_); }
    pvt->vfs->add_to_sys(loader_->create_sys_node());
}

// this method is called before add_to_sys
const bool Resource::init(void)
{
    pvt->vfs->init();

    // Initialize resources on VFS
    pvt->vfs->add_to_root(node);

    // create loaders(independent from resources)
    pvt->geom_loader = new GeomLoader;
    pvt->scrpt_loader = new ScrptLoader(pvt->vfs);
    pvt->var_loader = new VarLoader(pvt->vfs, pvt->define_root);
    add_loader(pvt->geom_loader);
    add_loader(pvt->scrpt_loader);
    add_loader(pvt->var_loader);

    // Log
    Node* log = new Node("Log", NULL, true);
    pvt->vfs->add_to_root(log);
    log->Add(g_log);

    RE_DBG("Initialized");

    return true;
}

const bool Resource::load(const Str& location_)
{
    // Read Filesystem
    for (list<Reader*>::iterator itr = pvt->reader_l.begin();
         itr != pvt->reader_l.end(); ++itr)
    { (*itr)->check(pvt->vfs, location_); }

    return true;
}

Node* Resource::make_define(void)
{
    pvt->fallback_mtl = new Material("Fallback");
    Node* fallback_tex = pvt->vfs->find("/System/Texture/Default");
    pvt->fallback_mtl->add_texture(dynamic_cast<Texture*>(fallback_tex));
    pvt->fallback_fn = dynamic_cast<Font*>
        (pvt->vfs->find("/System/Font/ReFont"));

    add_define(new Texture("Texture", 0, 0, 0, false, false));
    add_define(new Shader("Shader", false));
    add_define(new Material("Material"));
    add_define(new Geometry("Geometry"));
    // add_define(new Body("Body"));
    add_define(new Model("Model"));
    add_define(new Actor("Actor"));
    add_define(new Theme(Str("Theme"), pvt->fallback_mtl,
                         pvt->fallback_fn, pvt->fallback_fn, NULL));
    add_define(new Desktop(Str("Desktop"), Vec3(0, 0, 0), NULL, NULL));
    add_define(new HUD(pvt->vfs));
    add_define(new Scene("Scene"));
    add_define(new Space("Space"));
    add_define(new Stage("Stage"));
    add_define(new Generator("Generator"));

    return pvt->define_root;
}

const bool Resource::add_define(Node* define_)
{
    if (define_ == NULL || pvt->define_root == NULL) { return false; }
    pvt->define_root->Add(define_);
    pvt->define_ls.push_back(define_);

    return true;
}

VFS* Resource::get_vfs(void) const
{
    return pvt->vfs;
}

Loader* Resource::get_geom_loader(void) const
{
    return pvt->geom_loader;
}

void Resource::set_physics(Physics* phys_)
{
//     pvt->builder->set_physics(phys_);
}

void Resource::add_node(Node* node_)
{
    pvt->vfs->add_to_root(node_);
}

void Resource::reserve_sysres(void)
{
    RE_DBG("system resources reserving");

    // nodes for fallback
    Node* sys_node = pvt->vfs->find("/System");
    if (!sys_node) { return; }

    Node* mtl_node = new Node("Material", NULL, true);
    sys_node->Add(mtl_node);
    Material* default_mtl = new Material("Default");
    default_mtl->add_texture(dynamic_cast<Texture*>
                             (pvt->vfs->find("/System/Texture/Default")));
    mtl_node->Add(default_mtl);

    // from xml strings
    pvt->var_loader->read(def::sys_material, pvt->vfs, mtl_node);

    Node* theme_node = new Node("Theme", NULL, true);
    sys_node->Add(theme_node);
    pvt->var_loader->read(def::sys_theme, pvt->vfs, theme_node);

    Node* gen_node = new Node("Generator", NULL, true);
    sys_node->Add(gen_node);
    pvt->var_loader->read(def::sys_generator, pvt->vfs, gen_node);

    Node* mdl_node = new Node("Model", NULL, true);
    sys_node->Add(mdl_node);
    pvt->var_loader->read(def::sys_model, pvt->vfs, mdl_node);

    Node* actr_node = new Node("Actor", NULL, true);
    sys_node->Add(actr_node);
    pvt->var_loader->read(def::sys_actor, pvt->vfs, actr_node);

    Node* spc_node = new Node("Space", NULL, true);
    sys_node->Add(spc_node);
    pvt->var_loader->read(def::sys_space, pvt->vfs, spc_node);

    Node* scn_node = new Node("Scene", NULL, true);
    sys_node->Add(scn_node);
    pvt->var_loader->read(def::sys_scene, pvt->vfs, scn_node);

    Node* dsk_node = new Node("Desktop", NULL, true);
    sys_node->Add(dsk_node);
    pvt->var_loader->read(def::sys_desktop, pvt->vfs, dsk_node);

    Node* hud_node = new Node("HUD", NULL, true);
    sys_node->Add(hud_node);
    pvt->var_loader->read(def::sys_hud, pvt->vfs, hud_node);

    Node* stg_node = new Node("Stage", NULL, true);
    sys_node->Add(stg_node);
    pvt->var_loader->read(def::sys_stage, pvt->vfs, stg_node);

    RE_DBG("system loading completed");
}

void Resource::apply_default_config(void)
{
    pvt->var_loader->config(def::config_default, pvt->vfs, pvt->vfs->find("/"));
}
