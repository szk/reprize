#include "Common.hpp"
#include "Win32DepInfo.hpp"

#include "DIBase.hpp"

using namespace reprize;

LPDIRECTINPUT8 dinp = NULL;
LPDIRECTINPUTDEVICE8 dinp_key = NULL;
LPDIRECTINPUTDEVICE8 dinpMouse = NULL;
LPDIRECTINPUTDEVICE8 dinp_stick = NULL;

BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
{
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 
    
        // Set the range for the axis
        if( FAILED(dinp_stick->SetProperty( DIPROP_RANGE, &diprg.diph )) ) 
		{
//			console.err_message << "Can't set Poperty: OptIO" << endl;
            return DIENUM_STOP;
		}
    }

    return DIENUM_CONTINUE;
}

BOOL CALLBACK EnumOptIOsCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	// create device
	if ( FAILED(dinp->CreateDevice(pdidInstance->guidInstance, &dinp_stick, NULL)))
	{
//		console.err_message << "Can't create opt_io device: CreateDevice" << endl;

		return DIENUM_CONTINUE;
	}
	else return DIENUM_STOP;
}

DIBase::DIBase(Win32DepInfo* dep_info)
: InputBase(dep_info), win32_info(dep_info)
{
	win32_info->set_dinp(&dinp);
	win32_info->set_dinp_key(&dinp_key);
	win32_info->set_dinpMouse(&dinpMouse);
	win32_info->set_dinp_stick(&dinp_stick);
}

DIBase::~DIBase(void)
{
	if (dinp)
	{
//		console.sys_message << "DirectInput device released" << endl;

		dinp->Release();
		dinp = NULL;
	}
}

void DIBase::init(void)
{
	// create DirectInput
	if ( FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&dinp, NULL)) )
	{
//		input_initialized = FALSE;
	}

	// create keyboard device
	if ( FAILED(dinp->CreateDevice(GUID_SysKeyboard, &dinp_key, NULL)))
	{
//		input_initialized = FALSE;
	}
	// set default format
	else if ( FAILED(dinp_key->SetDataFormat(&c_dfDIKeyboard)))
	{
//		input_initialized = FALSE;
	}
	// set cooperativelevel
	else if ( FAILED(dinp_key->SetCooperativeLevel((win32_info->get_hwnd()), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)) )
	{
//		input_initialized = FALSE;
	}

	DIPROPDWORD di_prop = { sizeof(DIPROPDWORD), sizeof(DIPROPHEADER) };
	di_prop.diph.dwObj = 0;
	di_prop.diph.dwHow = DIPH_DEVICE;
	di_prop.dwData = DIPROPAXISMODE_REL;

	// init mouse
	if ( FAILED(dinp->CreateDevice(GUID_SysMouse, &dinpMouse, NULL)))
	{
		dinpMouse = NULL;
	}
	else if ( FAILED(dinpMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		dinpMouse = NULL;
	}
	else if ( FAILED(dinpMouse->SetCooperativeLevel( (win32_info->get_hwnd()), DISCL_EXCLUSIVE | DISCL_FOREGROUND )))
	{
		dinpMouse = NULL;
	}
	else if ( FAILED(dinpMouse->SetProperty( DIPROP_AXISMODE, &di_prop.diph )))
	{
		dinpMouse = NULL;
	}

	// enum
	if ( FAILED(dinp->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumOptIOsCallback, (LPVOID)((win32_info->get_hwnd())), DIEDFL_ATTACHEDONLY)) )
	{
		dinp_stick = NULL;
	}
	else if (dinp_stick == NULL) // It might create at EnumOptIOCallback
	{
		dinp_stick = NULL;
	}
	else if ( FAILED(dinp_stick->SetDataFormat( &c_dfDIOptIO2 )) )
	{
		dinp_stick = NULL;
	}
	else if ( FAILED(dinp_stick->SetCooperativeLevel( (win32_info->get_hwnd()), DISCL_EXCLUSIVE | DISCL_FOREGROUND )) )
	{
		dinp_stick = NULL;
	}
	else if ( FAILED(dinp_stick->EnumObjects( EnumObjectsCallback, (VOID*)((win32_info->get_hwnd())), DIDFT_ALL)) )
	{
		dinp_stick = NULL;
	}
}


void DIBase::restore(void)
{
}

void DIBase::check(void)
{
}
