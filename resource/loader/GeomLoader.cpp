#include "Common.hpp"

#include "resource/Mesh.hpp"
#include "resource/Geometry.hpp"
#include "GeomLoader.hpp"

#include "OBJ.hpp"
#include "util/nvtristrip/NvTriStrip.h"

using namespace reprize;
using namespace res;
using namespace std;

GeomLoader::GeomLoader(void)
    : Loader("GeometryLoader"), optimize(false)
{
}

GeomLoader::~GeomLoader(void)
{
}

Node* GeomLoader::create_sys_node(void)
{
    Geometry* ico = new Geometry("norm");
    ico->add_vertex(0, 0, 0, 0, 0); ico->add_vertex(1, 0, 0, 0, 0);
    ico->add_vertex(0, 1, 0, 0, 0); ico->add_vertex(0, 0, 1, 0, 0);
    Mesh* m1 = new Mesh(LINE_LOOP);
    m1->add_idx(0); m1->add_idx(1); m1->add_idx(2);
    Mesh* m2 = new Mesh(LINE_LOOP);
    m2->add_idx(0); m2->add_idx(1); m2->add_idx(3);
    Mesh* m3 = new Mesh(LINE_LOOP);
    m3->add_idx(0); m3->add_idx(1); m3->add_idx(3);
    Mesh* m4 = new Mesh(LINE_LOOP);
    m4->add_idx(0); m4->add_idx(2); m4->add_idx(3);
    ico->add_mesh(m1); ico->add_mesh(m2); ico->add_mesh(m3); ico->add_mesh(m4);

    Node* sys = new Node("Geometry");
    sys->Add(ico);

    return sys;
}

// this function is something wrong XXX
Geometry* GeomLoader::get_def_geometry(void)
{
    Geometry* def_geom = new Geometry("default");

    def_geom->add_vertex(0, 0, 0, 0, 0,  1, 1, 1, 1);
    def_geom->add_vertex(1, 0, 0, 0, 0,  1, 0, 0, 1);
    def_geom->add_vertex(0, 1, 0, 0, 0,  0, 1, 0, 1);
    def_geom->add_vertex(0, 0, 1, 0, 0,  0, 0, 1, 1);

    Mesh* m1 = new Mesh(TRIANGLE_STRIP);
    m1->add_idx(0); m1->add_idx(2); m1->add_idx(1);
    m1->add_idx(3); m1->add_idx(0);

    Mesh* m2 = new Mesh(TRIANGLE_STRIP);
    m2->add_idx(0); m2->add_idx(3); m2->add_idx(2);

    def_geom->add_mesh(m1);
    def_geom->add_mesh(m2);

    return def_geom;
}

const bool GeomLoader::load(const Str& filename_, const Str& path_)
{
    loaded_node = NULL;
    if (!compare_extension(filename_, ".obj")) { return false; }

    OBJ obj(path_);
    // if failed, cleanup existing geometry
    if (!obj.read()) { return false; }

    if (optimize)
    {
        vector<Mesh*>* optimized_mesh = optimize_tris(obj.get_tris());
        loaded_node = new Geometry(filename_, optimized_mesh,
                                   obj.get_vertex(), obj.get_uv());
    }
    else
    {
        loaded_node = new Geometry(filename_, obj.get_mesh(),
                                   obj.get_vertex(), obj.get_uv());
    }

    RE_DBG("%s, %s : %d %d", path_.c_str(), filename_.c_str(),
           obj.get_mesh()->size(), obj.get_vertex()->size());

    return true;
}

vector<Mesh*>* GeomLoader::optimize_tris(vector<uInt32>* tris_)
{
#ifdef INDEV
    unsigned short group;
    PrimitiveGroup* primitive_group = NULL;
    vector<Mesh*>* optimized_mesh = new vector<Mesh*>;
    vector<unsigned short> indices(tris_->begin(), tris_->end());

//    SetCacheSize(16);
//    SetStitchStrips(false);
//    SetMinStripSize(2);

    GenerateStrips(&(indices[0]), static_cast<const uInt32>(indices.size()), &primitive_group, &group);
//    GenerateStrips(&(tris_[0]), tris_->size(), &primitive_group, &group);

//     RE_DBG("source: ");
//     for (Size32 idx = 0; tris_->size() > idx; ++idx) { RE_DBG("%d", tris_->at(idx)); }

    // Replace the submeshes with the new one
    for (Size32 i = 0; i < group; ++i)
    {
        Mesh* opt_tris = NULL;

        switch (primitive_group[i].type)
        {
        case PT_LIST:
            opt_tris = new Mesh(TRIANGLES);
            optimized_mesh->push_back(opt_tris);
            break;
        case PT_STRIP:
            opt_tris = new Mesh(TRIANGLE_STRIP);
            optimized_mesh->push_back(opt_tris);
            break;
        case PT_FAN:
            opt_tris = new Mesh(TRIANGLE_FAN);
            optimized_mesh->push_back(opt_tris);
            break;
        }

//        RE_DBG("optimized: ");
        for (Size32 idx = 0; primitive_group[i].numIndices > idx; ++idx)
        {
            opt_tris->add_idx(primitive_group[i].indices[idx]);
//            RE_DBG("%d", primitive_group[i].indices[idx]);
        }
    }

    delete[] primitive_group;

    return optimized_mesh;
#endif
    return NULL;
}
