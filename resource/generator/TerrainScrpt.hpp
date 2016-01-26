#ifndef TERRAIN_SCRPT_HPP_
#define TERRAIN_SCRPT_HPP_

#include "resource/Node.hpp"

namespace  reprize
{
class TerrainScrpt : public res::Node
{
public:
    TerrainScrpt(void) : res::Node("Terrain", NULL) {}
    virtual ~TerrainScrpt(void) {}

    virtual const NID apply(const Str& txt_, const StrMap& am_) { return 0; }

private:
    const Str& create_geo(const StrMap& am_, Unit width_, Unit height_,
                          Unit rank_) { return missing; }

    Str name, missing;
};
}

#endif
