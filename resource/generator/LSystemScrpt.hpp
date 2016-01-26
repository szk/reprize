#ifndef L_SYSTEM_SCRPT_HPP_
#define L_SYSTEM_SCRPT_HPP_

#include "Common.hpp"
#include "resource/Script.hpp"
#include "resource/PropStr.hpp"

namespace  reprize
{
class LSystemScrpt : public res::Script // this should be create 'nodes'
{
public:
    LSystemScrpt(void)
        : Script(Str("LSystem"))
        {
        }
    virtual ~LSystemScrpt(void) {}

    virtual void apply(res::Props* props_, res::Node* tgt_)
        {
            const Str& type = props_->FindStr("type");
            const Vec3 begin(props_->FindStr("begin"));
            const Vec3 end(props_->FindStr("end"));
            const Vec3 dim(props_->FindStr("dim"));
//             const PrimFace face = get_face(props_->FindStr("face"));
            const Node* script_node = props_->FindNode("script");
        }

    virtual res::Props* gen_props(void)
        {
            RE_PROPS_START(LSystemScrpt);
            RE_PROP_NODE(LSystemScrpt, target);
            RE_PROP_STR(LSystemScrpt,  v);
            RE_PROP_STR(LSystemScrpt,  s);
            RE_PROP_STR(LSystemScrpt,  gamma);
            RE_PROP_STR(LSystemScrpt,  p);
            return RE_PROPS_FINISH;
        }

protected:
    const Str& prop_target(Node* arg_)
        {
            return RE_SUCCESS;
        }

    const Str& prop_v(const Str& arg_)
        {
//             script = dynamic_cast<Script*>(arg_);
//             if (!script) { return RE_FAILURE; }
            return RE_SUCCESS;
        }
    const Str& prop_s(const Str& arg_)
        {
            return RE_SUCCESS;
        }
    const Str& prop_gamma(const Str& arg_)
        {
            return RE_SUCCESS;
        }
    const Str& prop_p(const Str& arg_)
        {
            return RE_SUCCESS;
        }
};
}

#endif
