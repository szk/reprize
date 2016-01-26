#ifndef TOPOLO_GEN_HPP_
#define TOPOLO_GEN_HPP_

#include "resource/scene/Topology.hpp"
#include "resource/Script.hpp"

namespace reprize
{
namespace res
{
class TopoloGen : public Geometry
{
public:
    TopoloGen(const Str name_)
        : Geometry(name_), script(NULL)
        {
        }

    TopoloGen(const TopoloGen& src_)
        : Geometry(src_.name), script(NULL)
        {
        }

    virtual ~TopoloGen(void) {}

    virtual TopoloGen* Clone(const bool with_child_ = true)
        {
            return new TopoloGen(*this);
        }

    virtual res::Props* gen_props(void)
        {
            RE_PROPS_START(TopoloGen);
            RE_PROP_NODE(TopoloGen, script);
            RE_PROP_NODE(TopoloGen, geometry);
            RE_PROP_STR(TopoloGen,  type);
            RE_PROP_STR(TopoloGen,  begin);
            RE_PROP_STR(TopoloGen,  end);
            RE_PROP_STR(TopoloGen,  face);
            RE_PROP_STR(TopoloGen,  dim);
            return RE_PROPS_FINISH;
        }

    virtual void appendix(const Str& arg_, res::Props* props_)
        {
            if (script) { script->get_name().c_str(); }
            script->apply(props_, this);
        }

private:
    const Str& prop_script(Node* arg_)
        {
            script = dynamic_cast<Script*>(arg_);
            if (!script) { return RE_FAILURE; }
            return RE_SUCCESS;
        }
    const Str& prop_geometry(Node* arg_)
        {
            geom = dynamic_cast<Geometry*>(arg_);
            if (!geom) { return RE_FAILURE; }
            RE_DBG("topologen:geom");
            for (Size32 i = 0; geom->get_vertex_n() > i; ++i)
            {
                Vec3 vec = geom->get_vertex(i);
                RE_DBG("%d: %f, %f, %f",
                       i, vec.get_x(), vec.get_y(), vec.get_z());
            }
            return RE_SUCCESS;
        }
    const Str& prop_type(const Str& arg_)
        {
            return RE_SUCCESS;
        }
    const Str& prop_begin(const Str& arg_)
        {
            return RE_SUCCESS;
        }
    const Str& prop_end(const Str& arg_)
        {
            return RE_SUCCESS;
        }
    const Str& prop_dim(const Str& arg_)
        {
            return RE_SUCCESS;
        }
    const Str& prop_face(const Str& arg_)
        {
            return RE_SUCCESS;
        }
    Script* script;
    Geometry* geom;
};
}
}

#endif
