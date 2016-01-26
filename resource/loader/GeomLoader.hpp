#ifndef GEOM_LOADER_HPP_
#define GEOM_LOADER_HPP_

#include "Loader.hpp"

namespace reprize
{
namespace res
{
class Mesh;
class Geometry;

class GeomLoader : public Loader
{
public:
    GeomLoader(void);
    virtual ~GeomLoader(void);

    Node* create_sys_node(void);
    Geometry* get_def_geometry(void);

    const bool load(const Str& filename_, const Str& path_);

protected:
    std::vector<Mesh*>* optimize_tris(std::vector<uInt32>* tris_);
    bool optimize;
};
}
}
#endif
