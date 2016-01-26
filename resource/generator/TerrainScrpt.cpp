#include "Common.hpp"
#include "util/ImprovedNoise.hpp"

#include "TerrainScrpt.hpp"

using namespace reprize;
using namespace res;
using namespace std;

TerrainScrpt::TerrainScrpt(void)
{

}

TerrainScrpt::~TerrainScrpt(void)
{

}

const NID TerrainScrpt::apply(const Str& txt_, const StrMap& am_)
{
    name = ATT(am_, "name");
    RE_DBG("Terrain applying");
//     const Str type = ATT(am_, "type");
//     const Str geometry = ATT(am_, "geometry");
//     const Str texture = ATT(am_, "texture");
//     const Vec3 begin(ATT(am_, "begin"));
//     const Vec3 end(ATT(am_, "end"));
//     const Unit width = static_cast<Unit>
//         (strtod(ATT(am_, "width").c_str(), NULL));
//     const Unit height = static_cast<Unit>
//         (strtod(ATT(am_, "height").c_str(), NULL));

//     const uInt32 dim = strtol(ATT(am_, "dim").c_str(), NULL, 10);
//     const uInt32 xdim = strtol(ATT(am_, "xdim").c_str(), NULL, 10);
//     const uInt32 ydim = strtol(ATT(am_, "ydim").c_str(), NULL, 10);
//     const Unit size = static_cast<Unit>
//         (strtod(ATT(am_, "size").c_str(), NULL));
//     const Str body = ATT(am_, "body");
//     const PrimFace face = prim.get_face(ATT(am_, "face"));

//     if (name.empty()) { return missing; }

//     name = bldr_->model(Str("terrain"), Str("none"), Str("none"));
//     bldr_->set_parent(name);
    create_geo(bldr_, am_, 10, 10, 16);
    ImprovedNoise inoise;
    Float64 array[64];
    for (Size32 i = 0; i < 8; ++i)
    {
        for (Size32 j = 0; j < 8; ++j)
        {
            array[i + j * 7] = inoise.noise(static_cast<Float64>(j),
                                            static_cast<Float64>(i), 0.1);
        }
    }

    // bldr_->texture(Str("heightmap"), array, 8, TRILINEAR);
    RE_DBG("terrain created");

    return NID_NIL;

//     if (geom)
//     {
//         RE_DBG("TERRRAIN: " << geom->get_name() << endl;
//         return bldr_->geometry(geom);
//     }
//     missing = "/";
//     return missing;
}

const Str& TerrainScrpt::create_geo(const StrMap& am_,
                                    Unit width_, Unit height_, Unit rank_)
{
//     Primitive prim;

//     bldr_->set_parent(actor_str);
//     bldr_->model(Str("terrain_mdl"), Str("none"), Str("none"));

//     return name;

//     name = bldr_->actor(Str("Terrainu"), 0, 0, 0, 1, 1, 1);

//     bldr_->set_parent(name);
//     return name;

//     Str mdl_name = bldr_->model(Str("terrain_mdl"), Str("none"), Str("none"));
//     bldr_->set_parent(mdl_name);

//     res::Geometry* geom = prim.build_isometric(Str("terrain_geom"),
//                                                FACE_LINE, width_,
//                                                static_cast<uInt32>(rank_));
//     bldr_->geometry(geom);
// you need to check Entity::link and RenderState::add_mtt->
// these method pre-require available model.

/*
    bldr_->set_parent(name);
    bldr_->model(Str("terrainu_mdl"), Str("none"), Str("none"));
    bldr_->set_parent(mdl_name);
    res::Geometry* inu_geom = prim.build_isometric(Str("terrainu_geom"),
                                                   FACE_LINE, width_, rank_);
*/
//     bldr_->geometry(inu_geom);

//     bldr_->set_parent("Terrain");

//     name = bldr_->actor(Str("tarrrrain"), 1, 1, 1, 1, 1, 1);
//     bldr_->set_parent(name);
//     Str submdl_name = bldr_->model(Str("terrainhoge"), Str("none"), Str("none"));
//     bldr_->set_parent(mdl_name);
//     res::Geometry* geom = prim.build_isometric(Str("geom_name"),
//                                                FACE_LINE, width_, rank_);
//     bldr_->geometry(geom);

    return name;
//     return bldr_->geometry(geom);
}
/*
{
    Primitive prim;

    Str mdl_name = bldr_->model(Str("terrain_mdl"), Str("none"), Str("none"));
    bldr_->set_parent(mdl_name);

    res::Geometry* geom = prim.build_isometric(Str("terrain_geom"),
                                               FACE_LINE, width_, rank_);
    bldr_->geometry(geom);

    name = bldr_->actor(Str("Terrainu"), 0, 0, 0, 1, 1, 1);
    bldr_->set_parent(name);
    bldr_->model(Str("terrainu_mdl"), Str("none"), Str("none"));
    bldr_->set_parent(mdl_name);
    res::Geometry* inu_geom = prim.build_isometric(Str("terrainu_geom"),
                                                   FACE_LINE, width_, rank_);
    return name;
}
*/
