#ifndef MESH_HPP_
#define MESH_HPP_

namespace reprize
{
namespace res
{
enum PrmMode
{
    POINTS,
    LINES, LINE_LOOP, LINE_STRIP,
    TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN,
    QUADS, QUAD_STRIP, POLYGON,
    PRIM_NUM,
};

class Mesh
{
public:
    Mesh(PrmMode prm_mode_ = POINTS)
        : prm(prm_mode_), radius(0)
        {}

    ~Mesh(void) {}

    inline void add_idx(Size32 idx_) { idx.push_back(idx_); }
    inline const Size32 get_idx(Size32 n_) const { return idx.at(n_); }
    inline const Size32* get_idx_chunk(void) const { return &(idx.at(0)); }
    inline const Size32 get_idx_size(void) const { return idx.size(); }

    inline const PrmMode get_prm(void) const { return prm; }

    const Vec3& get_cog(void) { return cog; }
    void get_radius(void) {}
    void regulate_idx(void) {}

    void calc_cog(const Unit* vertex_chunk_)
        {
            Float64 hoge = 0;
            for (std::vector<Size32>::iterator m_idx = idx.begin();
                 m_idx != idx.end(); ++m_idx)
            {
                hoge += vertex_chunk_[(*m_idx)];
            }
            RE_DBG("chunk: %f", hoge);
        }

protected:
    const PrmMode prm;
    std::vector<Size32> idx;

    Vec3 cog; // center of gravity
    Unit radius;
    bool transparent;
};
}
}
#endif
