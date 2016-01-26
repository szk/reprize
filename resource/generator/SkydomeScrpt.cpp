#include "Common.hpp"
#include "SkydomeScrpt.hpp"

using namespace reprize;
using namespace res;
using namespace std;

SkydomeScrpt::SkydomeScrpt(void)
    : Node("Skydome", NULL)
{
    
}

SkydomeScrpt::~SkydomeScrpt(void)
{
    
}

const NID SkydomeScrpt::apply(const Str& txt_, const StrMap& am_)
{
    name = ATT(am_, "name");
    const Str type = ATT(am_, "type");
    const Str texture = ATT(am_, "texture");
    const Str altitude = ATT(am_, "altitude");
    const Str plants = ATT(am_, "plants");
    const Str temperature = ATT(am_, "temperature");
    const Str precipitation = ATT(am_, "precipitation");

    const Vec3 begin(ATT(am_, "begin"));
    const Vec3 end(ATT(am_, "end"));
//     const Unit width = static_cast<Unit>
//         (strtod(ATT(am_, "width").c_str(), NULL));
//     const uInt32 dim = strtol(ATT(am_, "dim").c_str(), NULL, 10);

//     if (name.empty()) { name = "/"; }
//     res::Geometry* geom = NULL;

    return NID_NIL;
}
/*
const bool init(void)
{
    const float fSize = 100.f;
    bool bResult = InitSkyParameters();
}

const bool update()
{
    UpdateSkyParameters();
}
*/
