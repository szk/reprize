#ifndef PRIMITIVE_SCRPT_HPP_
#define PRIMITIVE_SCRPT_HPP_

#include "Common.hpp"
#include "resource/Props.hpp"
#include "resource/Geometry.hpp"
#include "resource/Actor.hpp"
#include "resource/Script.hpp"
#include "resource/PropStr.hpp"

namespace  reprize
{
enum PrimFace { FACE_FRONT, FACE_BACK, FACE_LINE, FACE_POINT, FACE_NUM, };

class PrimitiveScrpt : public res::Script
{
public:
    PrimitiveScrpt(void)
        : Script(Str("Primitive")), ico_r(0), ico_i(0)
        {}
    virtual ~PrimitiveScrpt(void) {}

    virtual void apply(res::Props* props_, res::Node* tgt_)
        {
            res::Geometry* geom = dynamic_cast<res::Geometry*>(tgt_);
            if (props_ == NULL || geom == NULL)
            { RE_DBG("arguments are invalid"); return; }

            const Str& type = props_->FindStr("type");
            const Vec3 begin(props_->FindStr("begin"));
            const Vec3 end(props_->FindStr("end"));
            const Vec3 dim(props_->FindStr("dim"));
            const PrimFace face = get_face(props_->FindStr("face"));
            const Node* script_node = props_->FindNode("script");

            RE_DBG("applying primitive");
            if (type == Str("line")) { build_line(geom, begin, end); }
            else if (type == Str("rect"))
            { build_rect(geom, face, begin, end, begin, end); }
            else if (type == Str("circle"))
            { build_circle(geom, face, begin.get_x(), dim.get_x()); }
            else if (type == Str("plane"))
//             { build_plane(geom, face, width, height, xdim, ydim); }
            { build_plane(geom, face, begin.get_x(), begin.get_y(),
                          dim.get_x(), dim.get_y()); }
            else if (type == Str("isometric"))
            { build_isometric(geom, face, begin.get_x(), dim.get_x()); }
            else if (type == Str("box"))
            { build_box(geom, face, begin); }
            else if (type == Str("stripe"))
            { build_stripe(geom, face, begin); }
            else if (type == Str("ico"))
            {
                build_ico(geom, face, static_cast<Unit>(begin.get_x()),
                          static_cast<Unit>(dim.get_x()));
            }
        }

    PrimFace get_face(const Str& str_)
        {
            if (str_ == "back") { return FACE_BACK; }
            else if (str_ == "line") { return FACE_LINE; }
            else if (str_ == "point") { return FACE_POINT; }
            return FACE_FRONT;
        }

    res::Geometry* build_line(res::Geometry* tgt_,
                              const Vec3& begin_, const Vec3& end_)
        {

            tgt_->add_mesh(res::LINES);
            tgt_->add_mesh_idx(0);
            tgt_->add_mesh_idx(1);

            tgt_->add_vertex(begin_);
            tgt_->add_vertex(end_, 1, 1);

            return tgt_;
        }

    res::Geometry* build_rect(res::Geometry* tgt_, const PrimFace face_,
                              const Vec3& topleft_, const Vec3& bottomright_,
                              const Vec3& uv_topleft_,
                              const Vec3& uv_bottomright_)
        {

            tgt_->add_mesh(res::TRIANGLE_STRIP);

            for (uInt32 i = 0; 4 > i; ++i) { tgt_->add_mesh_idx(i); }

            Unit left = topleft_.get_x();
            Unit right = bottomright_.get_x();
            Unit top = topleft_.get_y();
            Unit bottom = bottomright_.get_y();

            Unit uv_left = uv_topleft_.get_x();
            Unit uv_right = uv_bottomright_.get_x();
            Unit uv_top = uv_topleft_.get_y();
            Unit uv_bottom = uv_bottomright_.get_y();

            tgt_->add_vertex(left, bottom, 0, uv_left, uv_bottom);
            tgt_->add_vertex(left, top, 0, uv_left, uv_top);
            tgt_->add_vertex(right, bottom, 0, uv_right, uv_bottom);
            tgt_->add_vertex(right, top, 0, uv_right, uv_top);

//             tgt_->add_vertex(left, top, 0, uv_left, uv_top);
//             tgt_->add_vertex(left, bottom, 0, uv_left, uv_bottom);
//             tgt_->add_vertex(right, top, 0, uv_right, uv_top);
//             tgt_->add_vertex(right, bottom, 0, uv_right, uv_bottom);

            return tgt_;
        }

    res::Geometry* build_circle(res::Geometry* tgt_, const PrimFace face_,
                                Unit r_, uInt32 slice_ = 8)
        {

            tgt_->add_mesh(res::LINE_LOOP);

            for (uInt32 i = 0; slice_ > i; ++i)
            {
                tgt_->add_mesh_idx(i);
                Unit x = sinf(static_cast<Unit>((PI * 2) * i / slice_));
                Unit z = cosf(static_cast<Unit>((PI * 2) * i / slice_));

                tgt_->add_vertex(x * r_, z * r_, 0.0);
            }

            return tgt_;
        }

    res::Geometry* build_plane(res::Geometry* tgt_, const PrimFace face_,
                               const Unit width_, const Unit height_,
                               const uInt32 x_slice_, const uInt32 y_slice_,
                               const Unit* z_height = NULL)
        {
            for (uInt32 i = 0; y_slice_ > i; ++i)
            {
                if (y_slice_ - 1 > i) { tgt_->add_mesh(res::TRIANGLE_STRIP); }

                for (uInt32 j = 0; x_slice_ > j; ++j)
                {
                    tgt_->add_vertex(static_cast<Unit>(j*(width_ / x_slice_)),
                                     static_cast<Unit>(i*(height_ / y_slice_)),
                                     0,
                                     (Unit)(x_slice_ - j) / (Unit)x_slice_,
                                     (Unit)(y_slice_ - i) / (Unit)y_slice_);
//z_height[z++]);
//                     if (&(z_height[z]))
//                     {
//                         add_vertex(j * (width_ / x_slice_),
//                                    i * (height_ / y_slice_), 50);
// //                        z_height[z++];
//                     }
//                     else
//                     {
//                         add_vertex(j * (width_ / x_slice_),
//                                    i * (height_ / y_slice_), 0);
//                     }

                    if (y_slice_ - 1 > i)
                    {
                        tgt_->add_mesh_idx(i * x_slice_ + j);
                        tgt_->add_mesh_idx((i + 1) * x_slice_ + j);
                    }
                }
            }
            return tgt_;
        }

    // builds plane by equilateral triangles.
    res::Geometry* build_isometric(res::Geometry* tgt_, const PrimFace face_,
                                   const Unit len_, const uInt32 rank_,
                                   const Unit* z_height_ = NULL)
        {
/*
            Unit r_uv[][2] = { {0, 1},   {0.5, 1},   {1, 1},
                               {0, 0.5}, {0.5, 0.5}, {1, 0.5},
                               {0, 0},   {0.5, 0},   {1, 0}, };

            Size32 uv_n = 0;
*/
            Size32 rand_idx = rand() % 9;


//             tgt_->add_mesh(res::TRIANGLE_STRIP);
            tgt_->add_mesh(res::LINE_STRIP);

//             Unit r = sqrt(3) / 6 * len_;
            Unit h = static_cast<Unit>(sqrtf(2.0f) / 2.0f * len_);
            Unit d = len_ / 2.0f;

            uInt32 rank_idx = 0, v_idx = 0;
            Unit offset_x = len_ * static_cast<Unit>(rank_) / 2;
            Unit offset_y = h * static_cast<Unit>(rank_);

            Unit u = 0, v = 0;

            for (uInt32 rank_y = 0; rank_y <= rank_; ++rank_y)
            {
                rank_idx = v_idx;
                // upward ^
                for (uInt32 rank_x = 0; rank_x <= (rank_ - rank_y); ++rank_x)
                {
                    Unit x = static_cast<Unit>(rank_x * len_) + (rank_y * d);
                    Unit y = static_cast<Unit>(rank_y * h);

//                     uv_idx[uv_n] = rand_idx;
                    rand_idx = get_uv_idx(rand_idx);

//                     if (rank_x % 2) { u = 0; }
//                     else { u = 0.5; }
//                     v += 0.5;
                    // caronical
                    u = 0.5f * rank_x;
                    v = 0.5f * rank_y;

                    tgt_->add_vertex(x - offset_x, y - offset_y, 0, u, v);

                    uInt32 first, second;
                    if (rank_y % 2) // odd
                    {
                        first = rank_idx - rank_x + (rank_ - rank_y);
                        second = rank_idx - rank_x + (rank_ - rank_y) * 2;

                        if (rank_x == (rank_ - rank_y))
                        {
                            ++second;
                            tgt_->add_mesh_idx(first);
                        }
                    }
                    else // even
                    {
                        first = v_idx;
                        second = v_idx + rank_ - rank_y + 1;
                        if (rank_x == (rank_ - rank_y))
                        {
                            --second;
                            tgt_->add_mesh_idx(first);
                        }
                    }

                    tgt_->add_mesh_idx(first);
                    tgt_->add_mesh_idx(second);
                    ++v_idx;
                }
            }
            return tgt_;
        }

    res::Geometry* build_stripe(res::Geometry* tgt_, const PrimFace face_,
                                const Vec3& size_)
        {
            Size32 div = 16, i = 0;
            for (i = 0; div >= i; ++i)
            {
                Unit depth = (size_.get_z() / div) * i;
                tgt_->add_vertex(0,             0,             depth, 0, 0);
                tgt_->add_vertex(size_.get_x(), 0,             depth, 1, 0);
                tgt_->add_vertex(0,             size_.get_y(), depth, 1, 1);
                tgt_->add_vertex(size_.get_x(), size_.get_y(), depth, 0, 1);
                // front
                tgt_->add_mesh(res::TRIANGLE_STRIP);
                tgt_->add_mesh_idx(i * 4);
                tgt_->add_mesh_idx(i * 4 + 1);
                tgt_->add_mesh_idx(i * 4 + 2);
                tgt_->add_mesh_idx(i * 4 + 3);
                // back
                tgt_->add_mesh(res::TRIANGLE_STRIP);
                tgt_->add_mesh_idx(i * 4);
                tgt_->add_mesh_idx(i * 4 + 2);
                tgt_->add_mesh_idx(i * 4 + 1);
                tgt_->add_mesh_idx(i * 4 + 3);
            }
            i *= 4;
            for (Size32 k = 0; div >= k; ++k)
            {
                Unit depth = (size_.get_z() / div) * k;
                tgt_->add_vertex(0,             depth,             0, 0, 0);
                tgt_->add_vertex(size_.get_x(), depth,             0, 1, 0);
                tgt_->add_vertex(0,             depth, size_.get_z(), 1, 1);
                tgt_->add_vertex(size_.get_x(), depth, size_.get_z(), 0, 1);
                // front
                tgt_->add_mesh(res::TRIANGLE_STRIP);
                tgt_->add_mesh_idx(i + k * 4);
                tgt_->add_mesh_idx(i + k * 4 + 1);
                tgt_->add_mesh_idx(i + k * 4 + 2);
                tgt_->add_mesh_idx(i + k * 4 + 3);
                // back
                tgt_->add_mesh(res::TRIANGLE_STRIP);
                tgt_->add_mesh_idx(i + k * 4);
                tgt_->add_mesh_idx(i + k * 4 + 2);
                tgt_->add_mesh_idx(i + k * 4 + 1);
                tgt_->add_mesh_idx(i + k * 4 + 3);
            }
            i *= 2;
            for (Size32 j = 0; div >= j; ++j)
            {
                Unit depth = (size_.get_z() / div) * j;
                tgt_->add_vertex(depth,             0,             0, 0, 0);
                tgt_->add_vertex(depth, size_.get_y(),             0, 1, 0);
                tgt_->add_vertex(depth,             0, size_.get_z(), 1, 1);
                tgt_->add_vertex(depth, size_.get_y(), size_.get_z(), 0, 1);
                // front
                tgt_->add_mesh(res::TRIANGLE_STRIP);
                tgt_->add_mesh_idx(i + j * 4);
                tgt_->add_mesh_idx(i + j * 4 + 1);
                tgt_->add_mesh_idx(i + j * 4 + 2);
                tgt_->add_mesh_idx(i + j * 4 + 3);
                // back
                tgt_->add_mesh(res::TRIANGLE_STRIP);
                tgt_->add_mesh_idx(i + j * 4);
                tgt_->add_mesh_idx(i + j * 4 + 2);
                tgt_->add_mesh_idx(i + j * 4 + 1);
                tgt_->add_mesh_idx(i + j * 4 + 3);
            }

            return tgt_;
        }

    res::Geometry* build_box(res::Geometry* tgt_, const PrimFace face_,
                             const Vec3& size_)
        {

            tgt_->add_vertex(0,             0,             0,             0, 0);
            tgt_->add_vertex(size_.get_x(), 0,             0,             1, 0);
            tgt_->add_vertex(0,             size_.get_y(), 0,             1, 1);
            tgt_->add_vertex(size_.get_x(), size_.get_y(), 0,             0, 1);

            tgt_->add_vertex(0,             0,             size_.get_z(), 0, 0);
            tgt_->add_vertex(size_.get_x(), 0,             size_.get_z(), 1, 0);
            tgt_->add_vertex(0,             size_.get_y(), size_.get_z(), 1, 1);
            tgt_->add_vertex(size_.get_x(), size_.get_y(), size_.get_z(), 0, 1);

            Size32 cup_idx[] = { 0, 1, 2, 3, 6, 7, 4, 5, 0, 1, 0, 2, 4, 6 };
            Size32 cap_idx[] = { 1, 5, 3, 7 };
            tgt_->add_mesh(res::TRIANGLE_STRIP);
            if (face_ == FACE_FRONT)
            {
                for (Size32 i = 0; i < 14; ++i)
                { tgt_->add_mesh_idx(cup_idx[i]); }
                tgt_->add_mesh(res::TRIANGLE_STRIP);
                for (Size32 i = 0; i < 4; ++i)
                { tgt_->add_mesh_idx(cap_idx[i]); }
            }
            else
            {
                for (Size32 i = 0; i < 14; ++i)
                { tgt_->add_mesh_idx(cup_idx[13 - i]); }
                tgt_->add_mesh(res::TRIANGLE_STRIP);
                for (Size32 i = 0; i < 4; ++i)
                { tgt_->add_mesh_idx(cap_idx[3 - i]); }
            }

            return tgt_;
        }

    // based on Platonic Solids (Regular polytopes in 3D)
    // http://astronomy.swin.edu.au/~pbourke/polyhedra/platonic/
    res::Geometry* build_ico(res::Geometry* tgt_, const PrimFace face_,
                             const Unit size_, const Unit dim_)
        {
            ico_i = 0;
            ico_r = size_;
            Unit l = dim_;
            if (l == 0) { l = 1; }
            // Geometry based on Paul Bourke's excellent article:
            //   Platonic Solids (Regular polytopes in 3D)
            //   http://astronomy.swin.edu.au/~pbourke/polyhedra/platonic/
            const Unit sqrt5 = sqrt(5.0f);
            const Unit phi = (1.0f + sqrt5) * 0.5f; // "golden ratio"
            // ratio of edge length to radius(ico_r)
            const Unit ratio = sqrt(10.0f + (2.0f * sqrt5)) / (4.0f * phi);
            const Unit a = (ico_r / ratio) * 0.5f;
            const Unit b = (ico_r / ratio) / (2.0f * phi);

            // define the icosahedron's 12 vertices:
            const Vec3  v1( 0,  b, -a),  v2( b,  a,  0),  v3(-b,  a,  0);
            const Vec3  v4( 0,  b,  a),  v5( 0, -b,  a),  v6(-a,  0,  b);
            const Vec3  v7( 0, -b, -a),  v8( a,  0, -b),  v9( a,  0,  b);
            const Vec3 v10(-a,  0, -b), v11( b, -a,  0), v12(-b, -a,  0);

            res::PrmMode m = res::TRIANGLE_STRIP;
            switch (face_)
            {
            case FACE_FRONT: m = res::TRIANGLE_STRIP; break;
            case FACE_BACK:  m = res::TRIANGLE_STRIP; break;
            case FACE_LINE:  m = res::LINE_STRIP; break;
            case FACE_POINT: m = res::POINTS; break;
            case FACE_NUM: m = res::PRIM_NUM; break;
            }

            // draw the icosahedron's 20 triangular faces:
            add_v_ico(tgt_, v1, v2, v3, l, m);   add_v_ico(tgt_, v4, v3, v2, l, m);
            add_v_ico(tgt_, v4, v5, v6, l, m);   add_v_ico(tgt_, v4, v9, v5, l, m);
            add_v_ico(tgt_, v1, v7, v8, l, m);   add_v_ico(tgt_, v1, v10, v7, l, m);
            add_v_ico(tgt_, v5, v11, v12, l, m); add_v_ico(tgt_, v7, v12, v11, l, m);
            add_v_ico(tgt_, v3, v6, v10, l, m);  add_v_ico(tgt_, v12, v10, v6, l, m);
            add_v_ico(tgt_, v2, v8, v9, l, m);   add_v_ico(tgt_, v11, v9, v8, l, m);
            add_v_ico(tgt_, v4, v6, v3, l, m);   add_v_ico(tgt_, v4, v2, v9, l, m);
            add_v_ico(tgt_, v1, v3, v10, l, m);  add_v_ico(tgt_, v1, v8, v2, l, m);
            add_v_ico(tgt_, v7, v10, v12, l, m); add_v_ico(tgt_, v7, v11, v8, l, m);
            add_v_ico(tgt_, v5, v12, v6, l, m);  add_v_ico(tgt_, v5, v9, v11, l, m);

            return tgt_;
        }

protected:
private:
    uInt32 get_uv_r_idx(uInt32 current_idx)
        {
            uInt32 r = rand();

            switch (current_idx)
            {
            case 0:
            { uInt32 avail0[] = { 1, 3, 4, 5 }; return avail0[r % 4]; }
            case 1:
            { uInt32 avail1[] = { 0, 2, 3, 4 }; return avail1[r % 4]; }
            case 2:
            { uInt32 avail2[] = { 1, 3, 4, 5 }; return avail2[r % 4]; }
            case 3:
            { uInt32 avail3[] = { 0, 1, 2, 4, 6 }; return avail3[r % 5];}
            case 4:
            { uInt32 avail4[] = { 1, 2, 3, 5, 6, 7 }; return avail4[r % 6]; }
            case 5: { uInt32 avail5[] = { 2, 4, 7, 8 }; return avail5[r % 4]; }
            case 6: { uInt32 avail6[] = { 3, 7 }; return avail6[r % 2]; }
            case 7: { uInt32 avail7[] = { 4, 5, 6, 8 }; return avail7[r % 4]; }
            case 8: { uInt32 avail8[] = { 3, 4,  5, 7 }; return avail8[r % 4]; }
            }
            return 0;
        }

    uInt32 get_uv_idx(Size32 current_idx)
        {
            uInt32 r = rand();

            switch (current_idx)
            {
            case 0: { return 7; }
            case 1: { uInt32 avail1[] = { 0, 2, 3, 4 }; return avail1[r % 4]; }
            case 2: { uInt32 avail2[] = { 1, 5 }; return avail2[r % 2]; }
            case 3: { return 1; }
            case 4: { return 2; }
            case 5: { uInt32 avail5[] = { 4, 8 }; return avail5[r % 2]; }
            case 6: { return 4; }
            case 7: { return 5; }
            case 8: { return 4; }
            }
            return 0;
        }

#ifdef UNDEF
    0 1 2
    3 4 5
    6 7 8

0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
 16  17  18  19  20  21  22  23  24  25  26  27  28  29  30
   31  32  33  34  35  36  37  38  39  40  41  42  43  44
     45  46  47  48  49  50  51  52  53  54  55  56  57
       58  59  60  61  62  63  64  65  66  67  68  69
         70  71  72  73  74  75  76  77  78  79  80
           81  82  83  84  85  86  87  88  89  90
             91  92  93  94  95  96  97  98  99
              100 101 102 103 104 105 106 107
                108 109 110 111 112 113 114
                  115 116 117 118 119 120
                    121 122 123 124 125
                      126 127 128 129
                        130 131 132
                          133 134
                            135

           9[0.5,2.5]

        7[0.5,2] 8[0,2] 

    4[0,1] 5[0.5,1.5] 6[0,1.5] 

0[0,0] 1[0.5,0.5] 2[0,0.5] 3[0.5,1] 

#endif

    inline Vec3 get_mid_on_sphere(const Vec3& a, const Vec3& b) const
        {
            Vec3 midpoint = (a + b) * 0.5f;
            midpoint.normalize();
            return midpoint * ico_r;
        }
    void add_v_ico(res::Geometry* tgt_,
                   const Vec3& a, const Vec3& b, const Vec3& c,
                   const Unit edge_len_, res::PrmMode meshmode_)
        {
            if ((((a - b).length()) < edge_len_) &&
                (((b - c).length()) < edge_len_) &&
                (((c - a).length()) < edge_len_))
            { add_ico_vert(tgt_, a, b, c, meshmode_); return; }

            const Vec3 ab = get_mid_on_sphere(a, b);
            const Vec3 bc = get_mid_on_sphere(b, c);
            const Vec3 ca = get_mid_on_sphere(c, a);

            add_v_ico(tgt_,  a, ab, ca, edge_len_, meshmode_);
            add_v_ico(tgt_, ab,  b, bc, edge_len_, meshmode_);
            add_v_ico(tgt_, ca, bc,  c, edge_len_, meshmode_);
            add_v_ico(tgt_, ab, bc, ca, edge_len_, meshmode_);
        }
#ifdef UNDEF
    void add_ico_vert(res::Geometry* tgt_,
                      const Vec3& a_, const Vec3& b_, const Vec3& c_,
                      res::PrmMode meshmode_)
        {
            tgt_->add_mesh(meshmode_);
            tgt_->add_vertex(a_, 0, 0, (a_ + Vec3(10, 10, 10)) / 20, 2.0);
//             tgt_->add_vertex(a_, 0, 0, Vec3(a_.get_x() / 20, 1, 1));
            tgt_->add_mesh_idx(ico_i++);
            tgt_->add_vertex(b_, 0, 0, (b_ + Vec3(10, 10, 10)) / 20, 2.0);
//             tgt_->add_vertex(b_, 0, 0, Vec3(b_.get_x() / 20, 1, 1));
            tgt_->add_mesh_idx(ico_i++);
            tgt_->add_vertex(c_, 0, 0, (c_ + Vec3(10, 10, 10)) / 20, 2.0);
//             tgt_->add_vertex(c_, 0, 0, Vec3(c_.get_x() / 20, 1, 1));
            tgt_->add_mesh_idx(ico_i++);

//             tgt_->add_mesh(res::LINE_STRIP);
//             tgt_->add_vertex(a_, 0, 0, Vec3(10, 10, 10));
//             tgt_->add_mesh_idx(i++);
//             tgt_->add_vertex(b_, 0, 0, Vec3(10, 10, 10));
//             tgt_->add_mesh_idx(i++);
//             tgt_->add_vertex(c_, 0, 0, Vec3(10, 10, 10));
//             tgt_->add_mesh_idx(i++);
        }
#endif

    void add_ico_vert(res::Geometry* tgt_,
                      const Vec3& a_, const Vec3& b_, const Vec3& c_,
                      res::PrmMode meshmode_)
        {
            tgt_->add_mesh(meshmode_);
            tgt_->add_vertex(a_, 0, 0, Vec3(0.6, 0.6, 0.6));
            tgt_->add_mesh_idx(ico_i++);
            tgt_->add_vertex(b_, 0, 0, Vec3(0.6, 0.6, 0.6));
            tgt_->add_mesh_idx(ico_i++);
            tgt_->add_vertex(c_, 0, 0, Vec3(0.6, 0.6, 0.6));
            tgt_->add_mesh_idx(ico_i++);

//             tgt_->add_mesh(res::LINE_STRIP);
//             tgt_->add_vertex(a_, 0, 0, Vec3(10, 10, 10));
//             tgt_->add_mesh_idx(i++);
//             tgt_->add_vertex(b_, 0, 0, Vec3(10, 10, 10));
//             tgt_->add_mesh_idx(i++);
//             tgt_->add_vertex(c_, 0, 0, Vec3(10, 10, 10));
//             tgt_->add_mesh_idx(i++);
        }

    Size32 uv_idx[136];

    // platonic
    Unit ico_r;
    uInt32 ico_i;
};
}

#endif

/*
    const NID apply(const StrMap& am_)
        {
            name = ATT(am_, "name");
            const Str type = ATT(am_, "type");
            const Vec3 begin(ATT(am_, "begin"));
            const Vec3 end(ATT(am_, "end"));
            const Unit width = static_cast<Unit>
                (strtod(ATT(am_, "width").c_str(), NULL));
            const Unit height = static_cast<Unit>
                (strtod(ATT(am_, "height").c_str(), NULL));

            const uInt32 dim = strtol(ATT(am_, "dim").c_str(), NULL, 10);
            const uInt32 xdim = strtol(ATT(am_, "xdim").c_str(), NULL, 10);
            const uInt32 ydim = strtol(ATT(am_, "ydim").c_str(), NULL, 10);
            const Unit size = static_cast<Unit>
                (strtod(ATT(am_, "size").c_str(), NULL));
            const Str body = ATT(am_, "body");
            const PrimFace face = get_face(ATT(am_, "face"));

            if (name.empty()) { name = "/"; return NID_NIL; }

//             geom = NULL;
            if (type == Str("line")) { geom = build_line(name, begin, end); }
            else if (type == Str("rect"))
            { build_rect(name, face, begin, end, begin, end); }
            else if (type == Str("circle"))
            { build_circle(name, face, size, dim); }
            else if (type == Str("plane"))
            { build_plane(name, face, width, height, xdim, ydim); }
            else if (type == Str("isometric"))
            { build_isometric(name, face, size, dim); }
            else if (type == Str("box"))
            { build_box(name, face, end); }
            else if (type == Str("stripe"))
            { build_stripe(name, face, begin); }
            else if (type == Str("ico"))
            {
                build_ico(name, face, static_cast<Unit>(size),
                          static_cast<Unit>(dim));
            }

            if (geom != NULL)
            {
//                 bldr_->geometry(geom);
                return NID_NIL;
            }

            return NID_NIL;
        }
*/
