#include "Common.hpp"
#include "Win32DepInfo.hpp"

#include "DIMouse.hpp"

using namespace reprize;

DIMouse::DIMouse(Win32DepInfo* dep_info)
: Mouse(dep_info), win32_info(dep_info)
{
    dinpMouse = win32_info->get_dinpMouse();
}

DIMouse::~DIMouse(void)
{
    if (dinpMouse)
    {
//		console.sys_message << "Keyboard released" << endl;

	(*dinpMouse)->Unacquire();
	(*dinpMouse)->Release();
	(*dinpMouse) = NULL;
    }
}

void DIMouse::init(void)
{
}

void DIMouse::restore(void)
{
    // reget mouse
    if (dinpMouse)
    {
//		if (dinpMouse->GetDeviceState(sizeof(kbuf), (void *)&kbuf) == DIERR_INPUTLOST )
	if (FAILED((*dinpMouse)->Acquire()))
	{
//			console.err_message << "Can't Acquire Mouse" << endl;
	}
    }
}

void DIMouse::get_pos(Vector3<Float32>* dst)
{
    DIMOUSESTATE2 mouse;

    if (dinpMouse)
    {
	if ( FAILED((*dinpMouse)->GetDeviceState( sizeof(DIMOUSESTATE2), &mouse )))
	{
	    (*dinpMouse)->Acquire();
	}
	else
	{
	    Int32 m_x = mouse.lX;
	    Int32 m_y = mouse.lY;
	    Int32 m_z = mouse.lZ;

	    bool m_button1 = ( ( mouse.rgbButtons[0] & 0x80 ) == 0x80 );
	    bool m_button2 = ( ( mouse.rgbButtons[1] & 0x80 ) == 0x80 );
	    bool m_button3 = ( ( mouse.rgbButtons[2] & 0x80 ) == 0x80 );

	    dst->set_var((Float32)m_x, (Float32)m_y, (Float32)m_z);
	    return;
	}
    }

    dst->set_var(0,0,0);
    return;
}

void DIMouse::get_motion(void)
{
}
