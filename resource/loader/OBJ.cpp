#include "Common.hpp"

#include "resource/Mesh.hpp"
#include "resource/Geometry.hpp"

#include "OBJ.hpp"

using namespace reprize;
using namespace res;
using namespace std;

OBJ::OBJ(Str filename_)
    : obj_stream(filename_.c_str()), max_vertex_idx(0), max_uv_idx(0),
      has_vt(false), has_vn(false)
{
    mesh = new vector<Mesh*>;
    tris = new vector<uInt32>;
    vertex = new vector<Unit>;
    uv = new vector<Unit>;
    normal = new vector<Unit>;
    color = new vector<Unit>;
}

OBJ::~OBJ(void)
{
//     DEL(mesh);
//     DEL(tris);
//     DEL(vertex);
//     DEL(uv);
}

const bool OBJ::is_correct(void)
{
    if (max_vertex_idx > vertex->size()) { return false; }
    if (max_uv_idx > uv->size()) { return false; }
    return true;
}

const bool OBJ::read(void)
{
    Str str;
    char buf[256];
    Str token;

    // get src data
    while (obj_stream.getline(buf, 256, '\n'))
    {
        stringstream ln(buf);
	ln >> token;
	if (token == "v") // vertex
	{
	    Unit v1, v2, v3;
	    ln >> v1; ln >> v2; ln >> v3;
	    vertex->push_back(v1);
	    vertex->push_back(v2);
	    vertex->push_back(v3);
	}
	else if (token == "vt") // uv
	{
            has_vt = true;
	    Unit vt1, vt2;
	    ln >> vt1; ln >> vt2;
	    uv_tmp.push_back(vt1);
	    uv_tmp.push_back(vt2);
	}
	else if (token == "vn") // normal(not supported)
	{
            has_vn = true;
 	    Unit vn1, vn2, vn3;
 	    ln >> vn1; ln >> vn2; ln >> vn3;
 	    normal->push_back(vn1);
 	    normal->push_back(vn2);
 	    normal->push_back(vn3);
	}
        else if (token == "f") // mesh
        {
            uInt32 v_idx, uv_idx, vn_idx;
            mesh->push_back(new Mesh(TRIANGLES));
            Mesh* current = mesh->back();

            while (ln.peek() == ' ') // XXX quick hack
            {
                ln >> v_idx;
                mesh_v.push_back(v_idx - 1);
                current->add_idx(v_idx - 1);

                if (has_vt)
                {
                    ln.ignore(); ln >> uv_idx;
                    mesh_uv.push_back(uv_idx - 1);
                }
                if (has_vn)
                {
                    ln.ignore(); ln >> vn_idx;
                    mesh_vn.push_back(vn_idx - 1);
                }
            }
        }
    }

    // uniform uv
    uv->resize((vertex->size() / 3) * 2, 0);
    uv_reserved.resize(mesh_v.size(), false);

    Size32 mv_max = mesh_v.size();
    Size32 muv_max = mesh_uv.size();
    for (Size32 m_idx = 0; mv_max > m_idx && muv_max > m_idx ; ++m_idx)
    {
        if (uv_reserved[mesh_v[m_idx]])
        {
//             mesh_v.push_back(uv->size());

//             vertex->push_back(vertex->at(mesh_v[m_idx] * 3));
//             vertex->push_back(vertex->at(mesh_v[m_idx] * 3 + 1));
//             vertex->push_back(vertex->at(mesh_v[m_idx] * 3 + 2));

//             uv->push_back(uv_tmp[mesh_uv[m_idx] * 2]); // u
//             uv->push_back(uv_tmp[mesh_uv[m_idx] * 2 + 1]); // v
        }
        else if (uv->size() > mesh_v[m_idx] * 2 + 1
                 && uv_tmp.size() > mesh_uv[m_idx] * 2 + 1)
        {
            uv->at(mesh_v[m_idx] * 2) = uv_tmp.at(mesh_uv[m_idx] * 2);// u
            uv->at(mesh_v[m_idx] * 2+1) = uv_tmp.at(mesh_uv[m_idx] * 2+1); // v
//            uv_reserved[mesh_v[m_idx]] = true;
        }

    }

    // create triangle data
    mv_max = mesh_v.size();
    for (Size32 m_idx = 0; mv_max > m_idx; ++m_idx)
    { tris->push_back(mesh_v[m_idx]); }

    RE_DBG("tris: %d", tris->size());
    return true;
}

vector<Mesh*>* OBJ::get_mesh(void)
{
    return mesh;
}

vector<Unit>* OBJ::get_vertex(void)
{
    return vertex;
}

vector<Unit>* OBJ::get_uv(void)
{
    return uv;
}

vector<uInt32>* OBJ::get_tris(void)
{
    return tris;
}
