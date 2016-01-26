#ifndef DI_MOUSE_HPP_
#define DI_MOUSE_HPP_

#include "Mouse.hpp"

namespace reprize
{
class DIMouse : public Mouse
{
public:
    DIMouse(Win32DepInfo* dep_info);
    virtual ~DIMouse(void);

    virtual void init(void);
    virtual void restore(void);

    virtual void get_pos(const Vector3<Float32>* dst);
    virtual void get_motion(void);

private:
    Win32DepInfo* win32_info;
    LPDIRECTINPUTDEVICE8* dinpMouse;
};
}
#endif
