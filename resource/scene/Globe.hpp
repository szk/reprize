#ifndef GLOBE_HPP_
#define GLOBE_HPP_

#include "PartSphere.hpp"

namespace reprize
{
namespace res
{

class Globe : public res::Entity
{
public:
    Globe(const Str name_, const bool up_)
        : Entity(name_), geom(NULL), mdl(NULL)
        {
            mtt = new res::Matter;
            geom = new res::Geometry("globe");
            mdl = new res::Model(Str("test"), geom);
            mtt->set_model(mdl);

            radius = 10;
            init();
            build_ico("ico", radius, 4);
        }
    virtual ~Globe(void) {}

    void init(void)
        {
            geom->add_mesh(res::LINE_LOOP);
            uInt32 lv = 32;
            for (uInt32 i = 0; lv > i; ++i)
            {
                Unit x = sinf(static_cast<Unit>((PI * 2) * i / lv));
                Unit z = cosf(static_cast<Unit>((PI * 2) * i / lv));
                geom->add_mesh_idx(i);
                geom->add_vertex(x * radius * 1.01f, z * radius * 1.01f,
                                 0, 0, 0, 1, 1, 0, 1);
            }
        }

    void build_ico(Str name_, const Unit size_, const Unit tri_len_)
        {
            Unit l = tri_len_;
            // Geometry based on Paul Bourke's excellent article:
            //  Platonic Solids (Regular polytopes in 3D)
            //  http://astronomy.swin.edu.au/~pbourke/polyhedra/platonic/
            const Unit sqrt5 = sqrt(5.0f);
            const Unit phi = (1.0f + sqrt5) * 0.5f; // "golden ratio"
            // ratio of edge length to radius
            const Unit ratio = sqrt(10.0f + (2.0f * sqrt5)) / (4.0f * phi);
            const Unit a = (size_ / ratio) * 0.5f;
            const Unit b = (size_ / ratio) / (2.0f * phi);

            // define the icosahedron's 12 vertices:
            const Vec3 v1( 0,  b, -a),  v2( b,  a,  0),  v3(-b,  a,  0);
            const Vec3 v4( 0,  b,  a),  v5( 0, -b,  a),  v6(-a,  0,  b);
            const Vec3 v7( 0, -b, -a),  v8( a,  0, -b),  v9( a,  0,  b);
            const Vec3 v10(-a,  0, -b), v11( b, -a,  0), v12(-b, -a,  0);

            res::PrmMode L = res::TRIANGLE_STRIP;

            p[0] = add_dome(v1, v2, v3, l, L, Vec3(1, 0, 0), 0);
            p[1] = add_dome(v4, v3, v2, l, L, Vec3(0, 1, 0), 1);
            p[2] = add_dome(v4, v5, v6, l, L, Vec3(0, 0, 1), 2);
            p[3] = add_dome(v4, v9, v5, l, L, Vec3(1, 1, 0), 3);
            p[4] = add_dome(v1, v7, v8, l, L,  Vec3(1, 0, 1), 4);
            p[5] = add_dome(v1, v10, v7, l, L, Vec3(0, 1, 1), 5);
            p[6] = add_dome(v5, v11, v12, l, L, Vec3(1, 0, 0), 6);
            p[7] = add_dome(v7, v12, v11, l, L, Vec3(0, 1, 0), 7);
            p[8] = add_dome(v3, v6, v10, l, L,   Vec3(1, 0.5, 1), 8);
            p[9] = add_dome(v12, v10, v6, l, L, Vec3(0, 0, 1), 9);
            p[10] = add_dome(v2, v8, v9, l, L,  Vec3(0.5, 1, 1), 10);
            p[11] = add_dome(v11, v9, v8, l, L, Vec3(1, 1, 0), 11);
            p[12] = add_dome(v4, v6, v3, l, L, Vec3(1, 1, 0.5), 12);
            p[13] = add_dome(v4, v2, v9, l, L, Vec3(1, 0.5, 0.5), 13);
            p[14] = add_dome(v1, v3, v10, l, L, Vec3(0.5, 1, 0.5), 14);
            p[15] = add_dome(v1, v8, v2, l, L, Vec3(0.5, 0.5, 1), 15);
            p[16] = add_dome(v7, v10, v12, l, L, Vec3(1, 0, 1), 16);
            p[17] = add_dome(v7, v11, v8, l, L, Vec3(0, 1, 1), 17);
            p[18] = add_dome(v5, v12, v6, l, L, Vec3(1, 0.5, 0.5), 18);
            p[19] = add_dome(v5, v9, v11, l, L, Vec3(0.5, 1, 0.5), 19);

            p[0]->set_sibling( p[15], p[14], p[1]);
            p[1]->set_sibling( p[12], p[13], p[0]);
            p[2]->set_sibling( p[3],  p[12], p[18]);
            p[3]->set_sibling( p[13], p[2],  p[19]);
            p[4]->set_sibling( p[5],  p[15], p[17]);
            p[5]->set_sibling( p[14], p[4], p[16]);
            p[6]->set_sibling( p[19], p[18], p[7]);
            p[7]->set_sibling( p[16], p[17],  p[6]);
            p[8]->set_sibling( p[12], p[14], p[9]);
            p[9]->set_sibling( p[16], p[18], p[8]);
            p[10]->set_sibling(p[15], p[13], p[11]);
            p[11]->set_sibling(p[19], p[17], p[10]);
            p[12]->set_sibling(p[2],  p[1],  p[8]);
            p[13]->set_sibling(p[1],  p[3],  p[10]);
            p[14]->set_sibling(p[0],  p[5],  p[8]);
            p[15]->set_sibling(p[4],  p[0],  p[10]);
            p[16]->set_sibling(p[5],  p[7],  p[9]);
            p[17]->set_sibling(p[7],  p[4], p[11]);
            p[18]->set_sibling(p[6],  p[2],  p[9]);
            p[19]->set_sibling(p[3],  p[6],  p[11]);

            current_part = p[0];
            return;
        }

    PartSphere* add_dome(const Vec3& a_, const Vec3& b_, const Vec3& c_,
                         const Unit edge_len_, res::PrmMode meshmode_,
                         const Vec3& color_, const Size32 idx_)
        {
            PartSphere* partsphere = new PartSphere("ps", color_, idx_);
            partsphere->init(a_, b_, c_, radius, edge_len_, meshmode_);
            Add(partsphere);

            return partsphere;
        }

    typedef res::PropStr<Globe> GlobePropStr;
/*
    void attach(void)
        {
            if (attached) { return; }
            Props* pr = new Props;
            pr->Add("goneg", new GlobePropStr(this, &Globe::prop_go_negative));
            pr->Add("gopos", new GlobePropStr(this, &Globe::prop_go_positive));
            pr->Add("goab",  new GlobePropStr(this, &Globe::prop_go_above));
//             Add(new Node("goab",  new GlobePropStr(this, &Globe::prop_go_above)));

            attached = true;
        }
*/
protected:
    const Str& prop_go_negative(const Str& arg_)
        {
            current_part->set_rel_pos(ps_pos);
            current_part = current_part->get_neg_sibling();
            ps_pos = current_part->get_rel_pos();
            current_part->set_rel_pos(Vec3(2, 1, 1));
            return RE_SUCCESS;
        }
    const Str& prop_go_positive(const Str& arg_)
        {
            current_part->set_rel_pos(ps_pos);
            current_part = current_part->get_pos_sibling();
            ps_pos = current_part->get_rel_pos();
            current_part->set_rel_pos(Vec3(2, 1, 1));
            return RE_SUCCESS;
        }

    const Str& prop_go_above(const Str& arg_)
        {
            current_part->set_rel_pos(ps_pos);
            current_part = current_part->get_ab_sibling();
            ps_pos = current_part->get_rel_pos();
            current_part->set_rel_pos(Vec3(2, 1, 1));
            return RE_SUCCESS;
        }

    res::Geometry* geom;
    res::Model* mdl;

    uInt32 geosphere, hydrosphere, atomosphere;

    bool first;
    // platonic
    PartSphere* p[20],* current_part;
    Unit radius;
    uInt32 i;
    Vec3 plate_col, ps_pos;
};
}
}

#endif

#ifdef UNDEF
    p[0] = add_dome(v1, v2, v3, l, O, Vec3(1, 0, 0)); //R
    p[1] = add_dome(v4, v3, v2, l, O, Vec3(0, 1, 0)); //G

    // facing vertical cap
    p[2] = add_dome(v4, v5, v6, l, O, Vec3(0, 0, 1)); //B
    p[3] = add_dome(v4, v9, v5, l, O, Vec3(1, 1, 0)); //RG

    // bacO vertical cap
    p[4] = add_dome(v1, v7, v8, l, O,  Vec3(1, 0, 1)); //RB
    p[5] = add_dome(v1, v10, v7, l, O, Vec3(0, 1, 1)); //GB

    // bottom horizontal cap
    p[6] = add_dome(v5, v11, v12, l, m, Vec3(1, 0, 0));
    p[7] = add_dome(v7, v12, v11, l, m, Vec3(0, 1, 0));

    // leftside vertical
    p[8] = add_dome(v3, v6, v10, l, O,   Vec3(1, 0.5, 1)); //RB high
    p[9] = add_dome(v12, v10, v6, l, m, Vec3(1, 1, 0));

    // rightside vertical
    p[10] = add_dome(v2, v8, v9, l, O,  Vec3(0.5, 1, 1)); // GB high
    p[11] = add_dome(v11, v9, v8, l, m, Vec3(1, 1, 1));

    // top left facing tilt
    p[12] = add_dome(v4, v6, v3, l, O, Vec3(1, 1, 0.5)); //RG high
    // top right facing tilt
    p[13] = add_dome(v4, v2, v9, l, O, Vec3(1, 0.5, 0.5)); //R high
    // top left bacO tilt
    p[14] = add_dome(v1, v3, v10, l, O, Vec3(0.5, 1, 0.5)); //G high
    // top right bacO tilt
    p[15] = add_dome(v1, v8, v2, l, O, Vec3(0.5, 0.5, 1)); //B high

    // // bottom left back tilt
    p[16] = add_dome(v7, v10, v12, l, m, Vec3(1, 1, 1));
    // bottom right back tilt
    p[17] = add_dome(v7, v11, v8, l, m, Vec3(1, 1, 1));
    // bottom left facing tilt
    p[18] = add_dome(v5, v12, v6, l, m, Vec3(1, 1, 1));
    // bottom right facing tilt
    p[19] = add_dome(v5, v9, v11, l, m, Vec3(1, 1, 1));
#endif

