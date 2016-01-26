#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include "Node.hpp"
#include "Mesh.hpp"

namespace reprize
{
namespace res
{
class Geometry : public Node
{
public:
    // read existing chunk
    Geometry(Str name_, std::vector<Mesh*>* mesh_ = NULL,
             std::vector<Unit>* vertex_ = NULL, std::vector<Unit>* uv_ = NULL,
             std::vector<Unit>* norm_ = NULL, std::vector<Unit>* color_ = NULL)
        : Node(name_, NULL), vertex_color(true), mesh(mesh_),
          vertex(vertex_), uv(uv_),  norm(norm_), color(color_)
        {
            mesh = mesh_ ? mesh_ : new std::vector<Mesh*>;
            vertex = vertex_ ? vertex_ : new std::vector<Unit>;
            uv = uv_ ? uv_ : new std::vector<Unit>;
            norm = norm_ ? norm_ : new std::vector<Unit>;

            if (color_) { color = color_; }
            else
            {
                color = new std::vector<Unit>;
                for (Size32 i = 0; vertex->size() /  3 > i; ++i)
                {
                    color->push_back(static_cast<Unit>(g_utl->rnd_f64()));
                    color->push_back(static_cast<Unit>(g_utl->rnd_f64()));
                    color->push_back(static_cast<Unit>(g_utl->rnd_f64()));
                    color->push_back(1);
                }
            }
        }

    virtual ~Geometry(void)
        { DEL(mesh); DEL(vertex); DEL(uv); DEL(norm); DEL(color); }

    inline const Mesh* get_mesh(const Size32 idx_) const
        { return mesh->at(idx_); }
    inline const Unit* get_vertex_chunk(void) const { return &(vertex->at(0)); }
    inline const Unit* get_uv_chunk(void) const { return &(uv->at(0)); }
    inline const Unit* get_norm_chunk(void) const { return &(norm->at(0)); }
    inline const Unit* get_color_chunk(void) const { return &(color->at(0)); }

    inline const Unit get_vertex_x(const Size32 idx_) const
        { return vertex->at(idx_ * 3); }
    inline const Unit get_vertex_y(const Size32 idx_) const
        { return vertex->at(idx_ * 3 + 1); }
    inline const Unit get_vertex_z(const Size32 idx_) const
        { return vertex->at(idx_ * 3 + 2); }
    inline const Vec3 get_vertex(const Size32 idx_) const
        {
            Size32 n = idx_ * 3;
            return Vec3(vertex->at(n), vertex->at(n + 1), vertex->at(n + 2));
        }
    inline const Unit* get_vertex_array(const Size32 idx_) const
        { return &(vertex->at(idx_ * 3)); }

    inline void set_vertex_x(const Int32 idx_, const Unit x_)
        { vertex->at(idx_ * 3) = x_; }
    inline void set_vertex_y(const Int32 idx_, const Unit y_)
        { vertex->at(idx_ * 3 + 1) = y_; }
    inline void set_vertex_z(const Int32 idx_, const Unit z_)
        { vertex->at(idx_ * 3 + 2) = z_; }
    inline void set_vertex(const Int32 idx_, const Vec3& pos_)
        {
            Size32 n = idx_ * 3;
            vertex->at(n) = pos_.get_x();
            vertex->at(n + 1) = pos_.get_y();
            vertex->at(n + 2) = pos_.get_z();
        }

    inline const Unit get_uv_u(const Size32 idx_) const
        { return uv->at(idx_ * 2); }
    inline const Unit get_uv_v(const Size32 idx_) const
        { return uv->at(idx_ * 2 + 1); }

    inline void set_uv_u(const Int32 idx_, const Unit u_)
        { uv->at(idx_ * 2) = u_; }
    inline void set_uv_v(const Int32 idx_, const Unit v_)
        { uv->at(idx_ * 2 + 1) = v_; }

    inline const Unit get_color_r(const Size32 idx_) const
        { return color->at(idx_ * 4); }
    inline const Unit get_color_g(const Size32 idx_) const
        { return color->at(idx_ * 4 + 1); }
    inline const Unit get_color_b(const Size32 idx_) const
        { return color->at(idx_ * 4 + 2); }
    inline const Unit get_color_a(const Size32 idx_) const
        { return color->at(idx_ * 4 + 3); }
    inline Vec3 get_color(const Size32 idx_) const
        {
            Size32 n = idx_ * 4;
            return Vec3(color->at(n), color->at(n + 1), color->at(n + 2));
        }

    inline void set_color_r(const Size32 idx_, const Unit r_)
        { color->at(idx_ * 4) = r_; }
    inline void set_color_g(const Size32 idx_, const Unit g_)
        { color->at(idx_ * 4 + 1) = g_; }
    inline void set_color_b(const Size32 idx_, const Unit b_)
        { color->at(idx_ * 4 + 2) = b_; }
    inline void set_color_a(const Size32 idx_, const Unit a_)
        { color->at(idx_ * 4 + 3) = a_; }
    inline void set_color(const Size32 idx_, const Vec3& col_)
        {
            Size32 n = idx_ * 4;
            color->at(n) = col_.get_x();  color->at(n + 1) = col_.get_y();
            color->at(n + 2) = col_.get_z(); color->at(n + 3) = 1;
        }

    inline const Size32 get_mesh_size(void) const { return mesh->size(); }
    inline const Size32 get_vertex_size(void) const { return vertex->size(); }
    inline const Size32 get_vertex_n(void) const { return vertex->size() / 3; }
    inline const Size32 get_uv_size(void) const { return uv->size(); }
    inline const Size32 get_uv_n(void) const { return uv->size() / 2; }
    inline const Size32 get_color_size(void) const { return color->size(); }
    inline const Size32 get_color_n(void) const { return color->size() / 4; }

    inline void add_mesh(Mesh* mesh_) { mesh->push_back(mesh_); }
    inline void add_mesh(PrmMode prm_) { mesh->push_back(new Mesh(prm_)); }
    inline const bool add_mesh_idx(Size32 idx_)
        {
            if (mesh->empty())
            {
                RE_DBG("can't add mesh idx: %s", name.c_str());
                return false;
            }
            mesh->back()->add_idx(idx_);
            return true;
        }

    inline void add_vertex(const Unit x_, const Unit y_, const Unit z_,
                           const Unit u_ = 0, const Unit v_ = 0,
                           const Unit r_ = 1, const Unit g_ = 1,
                           const Unit b_ = 1, const Unit a_ = 1)
        {
            vertex->push_back(x_); vertex->push_back(y_); vertex->push_back(z_);
            uv->push_back(u_); uv->push_back(v_);
            color->push_back(r_); color->push_back(g_);
            color->push_back(b_); color->push_back(a_);
        }

    inline void add_vertex(const Vec3& vertex_,
                           const Unit u_ = 0, const Unit v_ = 0,
                           const Vec3& color_ = Vec3(1, 1, 1),
                           const Unit a_ = 1)
        {
            add_vertex(vertex_.get_x(), vertex_.get_y(), vertex_.get_z(), u_,
                       v_, color_.get_x(), color_.get_y(), color_.get_z(), a_);
        }

    inline const bool regulate(void) { return false; }

    inline void offset(const Unit x_, const Unit y_, const Unit z_)
        {
            for (Size32 i = 0; vertex->size() / 3 > i; ++i)
            {
                vertex->at(i * 3) += x_;
                vertex->at(i * 3 + 1) += y_;
                vertex->at(i * 3 + 2) += z_;
            }
        }

    inline void rotate(const Unit x_axis_, const Unit y_axis_,
                       const Unit z_axis_)
        {
            Mtx44 mtx;
            mtx.quat(x_axis_, y_axis_, z_axis_);

            for (Size32 i = 0; vertex->size() / 3 > i; ++i)
            {
                Mtx44 v_mtx(Vec3(vertex->at(i * 3),
                                 vertex->at(i * 3 + 1),
                                 vertex->at(i * 3 + 2)));
                v_mtx.mlt(mtx);

                vertex->at(i * 3) = v_mtx.get_pos().get_x();
                vertex->at(i * 3 + 1) = v_mtx.get_pos().get_y();
                vertex->at(i * 3 + 2) = v_mtx.get_pos().get_z();
            }
        }

    virtual void set_culling(const Vec3& viewpoint_) const
        {
        }
    virtual void revert_culling(void) const
        {
        }
    const bool is_valid(void) const
        {
            return mesh && mesh->size() && vertex && vertex->size()
                && uv && uv->size();
        }
    inline void b_union(const Geometry* geom_)
        { RE_DBG("boolian union is unavailable"); }
    inline void b_difference(const Geometry* geom_)
        { RE_DBG("boolian difference is unavailable"); }
    inline void b_intersection(const Geometry* geom_)
        { RE_DBG("boolian intersection is unavailable"); }

    inline void extrude(const Vec3& dst_) {}

    inline const bool is_vertex_color(void) const { return vertex_color; }
    inline const Unit get_circumscription(void) const
        {
            if (!is_valid()) { return 0; }
            Unit o = 0;
            for (Size32 n = get_vertex_n(); n != 0; --n)
            { o = RE_MAX(get_vertex(n - 1).length(), o); }

            return o;
        }

protected:
    /*
    typedef PropStr<Geometry> GeomPropStr;
    void attach(void)
        {
            if (attached) { return; }
            Props* pr = new Props;
            pr->Add("add_mesh",     new GeomPropStr(this, &Geometry::prop_add_mesh));
            pr->Add("add_mesh_idx", new GeomPropStr(this, &Geometry::prop_add_mesh_idx));
            pr->Add("add_vertex",   new GeomPropStr(this, &Geometry::prop_add_vertex));
            pr->Add("vertex_size",  new GeomPropStr(this, &Geometry::prop_vertex_size));
            pr->Add("mesh_size",    new GeomPropStr(this, &Geometry::prop_mesh_size));
            attached = true;
        }

*/
    const Str& prop_add_mesh(const Str& arg_)
        {
            if (arg_ == Str("POINTS")) { add_mesh(POINTS); }
            else if (arg_ == Str("LINES")) { add_mesh(LINES); }
            else if (arg_ == Str("LINE_LOOP")) { add_mesh(LINE_LOOP); }
            else if (arg_ == Str("LINE_STRIP")) { add_mesh(LINE_STRIP); }
            else if (arg_ == Str("TRIANGLES")) { add_mesh(TRIANGLES); }
            else if (arg_ == Str("TRIANGLE_STRIP"))
            { add_mesh(TRIANGLE_STRIP); }
            else if (arg_ == Str("TRIANGLE_FAN")) { add_mesh(TRIANGLE_FAN); }
            else if (arg_ == Str("QUADS")) { add_mesh(QUADS); }
            else if (arg_ == Str("QUAD_STRIP")) { add_mesh(QUAD_STRIP); }
            else if (arg_ == Str("POLYGON")) { add_mesh(POLYGON); }

            return RE_SUCCESS;
        }
    const Str& prop_add_mesh_idx(const Str& arg_)
        {
            Vec3 dec(arg_);
            add_mesh_idx(static_cast<Size32>(dec.get_x()));
            return RE_SUCCESS;
        }
    const Str& prop_add_vertex(const Str& arg_)
        {
            add_vertex(Vec3(arg_));
            return RE_SUCCESS;
        }

    const Str& prop_vertex_size(const Str& arg_)
        {
            RE_DBG("%d", vertex->size());
            return RE_SUCCESS;
        }
    const Str& prop_mesh_size(const Str& arg_)
        {
            RE_DBG("%d", mesh->size());
            return RE_SUCCESS;
        }

    const bool vertex_color;
    std::vector<Mesh*>* mesh;
    std::vector<Unit>* vertex,* uv,* norm,* color;
};
}
}
#endif
