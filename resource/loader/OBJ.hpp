#ifndef OBJ_HPP_
#define OBJ_HPP_

namespace reprize
{
namespace res
{
class OBJ
{
public:
    OBJ(Str filename_);
    virtual ~OBJ(void);

    const bool is_correct(void);

    const bool read(void);

    std::vector<Mesh*>* get_mesh(void);
    std::vector<Unit>* get_vertex(void);
    std::vector<Unit>* get_uv(void);
    std::vector<uInt32>* get_tris(void);

protected:
    std::ifstream obj_stream;

    std::vector<uInt32> mesh_v, mesh_uv, mesh_vn;
    std::vector<bool> uv_reserved;
    std::vector<Unit> uv_tmp;

    std::vector<Mesh*>* mesh;
    std::vector<uInt32>* tris;
    std::vector<Unit>* vertex,* uv,* normal,* color;

    uInt32 max_vertex_idx, max_uv_idx;
    bool has_vt, has_vn;
};
}
}
#endif
