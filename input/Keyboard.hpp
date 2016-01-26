#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include "InputBase.hpp"
#include "InputBuffer.hpp"

#define STR_BUF_SIZE 512

class EnvDepInfo;

namespace reprize
{
namespace inp
{
class Keyboard
{
public:
    Keyboard(EnvDepInfo* dep_info) { RE_ZERO(strbuf, STR_BUF_SIZE); }
    virtual ~Keyboard(void) {}

    virtual const bool init(void) { return false; }
    virtual void restore(void) {}
    virtual void reset(void)
        {
            // strings
            strbuf[0] = NULCHAR;
            strbuf_len = 0;
            // keys
            mod = K_MOD_NONE;
        }

    InputCode get_mod(void) { return mod; }
    const Char* get_str(void) { return strbuf; }

    virtual void check(InputBuffer& pressed_, InputBuffer& released_) {}

protected:
    inline void add_key(InputCode keycode_, InputBuffer& buf_)
        {
            buf_.Add(keycode_);
        }

    Char strbuf[STR_BUF_SIZE];
    Size32 strbuf_len;
    InputCode mod;
};
}
}
#endif
