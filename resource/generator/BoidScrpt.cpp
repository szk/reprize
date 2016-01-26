#include "Common.hpp"
#include "BoidScrpt.hpp"

using namespace reprize;
using namespace res;
using namespace std;

BoidScrpt::BoidScrpt(void)
{
}

BoidScrpt::~BoidScrpt(void)
{
}

const NID BoidScrpt::apply(const Str& txt_, const StrMap& am_)
{
#ifdef UNDEF
    Str name = ATT(am_, "name");
    Str offset = ATT(am_, "offset");
    std::stringstream ln(offset);
    Unit x = 0, y = 0, z = 0, w = 1, h = 1, d = 1;

    /*
      RE_DBG("boid though");

    Primitive prim;
    Str mdl_name = bldr_->model(Str("boid_mdl"), Str("none"), Str("none"));
    bldr_->set_parent(mdl_name);

    res::Geometry* geom = prim.build_box(Str("terrain_geom"),
                                         FACE_FRONT, Vec3(3, 3, 3));
//     res::Geometry* geom = prim.build_ico(Str("boid_geom"), FACE_FRONT, 3, 3);
    bldr_->geometry(geom);
    */
    return bldr_->actor(name, x, y, z, w, h, d);
#endif
    return 0;
}
