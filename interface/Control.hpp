#ifndef CONTROL_HPP_
#define CONTROL_HPP_

#include "input/InputBase.hpp"
#include "input/InputBuffer.hpp"

namespace reprize
{
namespace inf
{
class Control : public res::Node
{
public:
    Control(void) : Node("Control")
        {
            RE_ZERO(binded_prop, INPUT_MAX_CODE);
        }
    virtual ~Control(void) { DEL(entry); }

    virtual const bool init(inp::Input* input_)
        {
            input = input_;
            return true;
        }

    virtual res::Props* gen_props(void)
        {
            RE_PROPS_START(Control);
            RE_PROP_STR(Control, bind);
            return RE_PROPS_FINISH;
        }

    const bool set_prop(res::Props* pr_)
        {
            pr = pr_;
            return false;
        }

    const bool key(inp::InputBuffer& pressed_, inp::InputBuffer& released_)
        {
            for (pressed_.First(); !pressed_.IsDone(); pressed_.Next())
            {
                if (pressed_.Current() == INPUT_MAX_CODE)
                { RE_DBG("invalid key pressed"); continue; }
                if (binded_prop[pressed_.Current()] != NULL)
                { binded_prop[pressed_.Current()]->Apply(PROP_TOGGLE_ON); }
                else { RE_DBG("Keycode %d is unbound", pressed_.Current()); }
            }

            for (released_.First(); !released_.IsDone(); released_.Next())
            {
                if (released_.Current() == INPUT_MAX_CODE)
                { RE_DBG("invalid key released"); continue; }
                if (binded_prop[released_.Current()] != NULL)
                { binded_prop[released_.Current()]->Apply(PROP_TOGGLE_OFF); }
            }
            return true;
        }

private:
    const Str& prop_bind(const Str& arg_)
        {
            if (arg_.empty()) { return RE_FAILURE; }

            std::stringstream ss(arg_);
            std::stringbuf name_buf;
            Str key_tok, prop_tok;
            getline(ss, key_tok, ' ');
            getline(ss, prop_tok, ' ');
            InputCode key_code = input->str_to_code(key_tok);

            // if the argument include a keycode only (for key binding from GUI)
            if (RE_ISNUM(key_tok) && prop_tok.empty())
            {
                RE_DBG("input code is number:%d", key_code);
                return inpcode_to_inpname(key_code);
            }

            // read the command
            if (prop_tok.empty()) { return RE_FAILURE; }

            res::Property* dst_prop = pr->Find(prop_tok);
            if (!dst_prop)
            {
                RE_DBG("bind failed: %s", prop_tok.c_str());
                binded_prop[key_code] = NULL;
                return RE_FAILURE;
            }
            binded_prop[key_code] = dst_prop;
            std::map<const Str, const Str*>::iterator itr =
                binds.find(prop_tok);
            if (itr != binds.end()) { binds.erase(itr); }
            binds.insert(make_pair(prop_tok, &(input->code_to_name(key_code))));

            return input->code_to_name(key_code);
        }

    const Str* propname_to_inpname(const Str& prop_name_)
        {
            std::map<const Str, const Str*>::iterator itr =
                binds.find(prop_name_);
            if (itr == binds.end()) { return &RE_FAILURE; }
            return itr->second;
        }

    const Str& inpcode_to_inpname(InputCode code_)
        { return input->code_to_name(code_); }

    inp::Input* input;
    res::Props* pr;
    res::Property* binded_prop[INPUT_MAX_CODE];
    std::map<const Str, const Str*> binds;
};

}
}

#endif
