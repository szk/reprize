#ifndef WMSG_KEYBOARD_HPP_
#define WMSG_KEYBOARD_HPP_

#include "Keyboard.hpp"

namespace reprize
{
namespace inp
{
#define K_BUF_SIZE 512

class WMsgKeyboard : public Keyboard
{
public:
    WMsgKeyboard(reprize::Win32DepInfo* dep_info_);
    virtual ~WMsgKeyboard(void);

    virtual const bool init(void);
    virtual void restore(void);
    virtual void reset(void);
    virtual void check(InputBuffer& pressed_, InputBuffer& released_);

private:
    InputCode translate_keycode(UINT win32_keycode_);
    reprize::Win32DepInfo* win32_info;
    uInt32 meta_mask, numlock_mask;

    BYTE kstate[256], post_kstate[256];
    std::map<UINT, InputCode> win32_keycodes;
};
}
}
#endif
