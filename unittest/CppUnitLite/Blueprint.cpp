#include "Common.hpp"

#include "resource/loader/GeomLoader.hpp"
#include "resource/loader/TexLoader.hpp"
#include "resource/loader/FontLoader.hpp"
#include "resource/Actor.hpp"
#include "resource/Model.hpp"
#include "resource/Geometry.hpp"
#include "resource/Text.hpp"

#include "physics/Sphere.hpp"

using namespace reprize;
using namespace res;

Blueprint::Blueprint(const Str& name_)
    : name(name_), crawling_child(false)
{
}

Blueprint::~Blueprint(void)
{
}

const Str& Blueprint::apply(Builder* bldr_, const Str& txt_, const StrMap& am_)
{
    return name;
}

const Str& Blueprint::get_name(void) const
{
    return name;
}

const bool Blueprint::is_crawling(void) const
{
    return crawling_child;
}
