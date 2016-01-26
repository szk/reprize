#ifndef DI_KEYBOARD_HPP_
#define DI_KEYBOARD_HPP_

#include "Keyboard.hpp"

namespace reprize
{
class DIKeyboard : public Keyboard
{
public:
    DIKeyboard(Win32DepInfo* dep_info);
    virtual ~DIKeyboard(void);

    virtual void init(void);
    virtual void restore(void);
    virtual uInt32 get_key(void);

private:
    Win32DepInfo* win32_info;
    LPDIRECTINPUTDEVICE8* dinp_key;
};
}
#endif
