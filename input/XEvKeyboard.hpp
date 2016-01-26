#ifndef XEV_KEYBOARD_HPP_
#define XEV_KEYBOARD_HPP_

#include "Keyboard.hpp"

#define K_BUF_SIZE 512

namespace reprize
{
class X11DepInfo;

namespace inp
{
class XEvKeyboard : public Keyboard
{
public:
    XEvKeyboard(X11DepInfo* dep_info_);
    virtual ~XEvKeyboard(void);

    virtual const bool init(void);
    virtual void restore(void);
    virtual void reset(void);
    virtual void check(InputBuffer& pressed_, InputBuffer& released_);

protected:
    InputCode translate_keycode(KeyCode x_keycode_);
    X11DepInfo* x11_info;
    Time pressed_time, released_time;
    KeyCode pressed_keycode, released_keycode; // for repeat detecting
    bool repeat, post_repeat;
    uInt32 meta_mask, numlock_mask;
    uChar key_buf[K_BUF_SIZE];

    char kstate[32], post_kstate[32];
    std::map<KeyCode, InputCode> x_keycode_map;
};
}
}

#endif
