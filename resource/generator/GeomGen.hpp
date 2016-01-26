#ifndef GEOM_GEN_HPP_
#define GEOM_GEN_HPP_

#include "resource/Geometry.hpp"
#include "resource/Script.hpp"

namespace reprize
{
namespace res
{
class GeomGen : public Geometry
{
public:
    GeomGen(const Str name_)
        : Geometry(name_), script(NULL)
        {
        }

    GeomGen(const GeomGen& src_)
        : Geometry(src_.name), script(NULL)
        {
        }

    virtual ~GeomGen(void) {}

    virtual GeomGen* Clone(const bool with_child_ = true)
        {
            return new GeomGen(*this);
        }

    virtual res::Props* gen_props(void)
        {
            RE_PROPS_START(GeomGen);
            RE_PROP_NODE(GeomGen, script);
            RE_PROP_STR(GeomGen,  type);
            RE_PROP_STR(GeomGen,  begin);
            RE_PROP_STR(GeomGen,  end);
            RE_PROP_STR(GeomGen,  face);
            RE_PROP_STR(GeomGen,  dim);
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
    const Str& prop_type(const Str& arg_)
        {
            return RE_SUCCESS;
        }
    const Str& prop_begin(const Str& arg_)
        {
            RE_DBG("hoge: %s", arg_.c_str());
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
};
}
}

#endif
