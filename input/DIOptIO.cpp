#include "Common.hpp"
#include "Win32DepInfo.hpp"

#include "DIOptIO.hpp"

using namespace reprize;

DIOptIO::DIOptIO(Win32DepInfo* dep_info)
    : OptIO(dep_info), win32_info(dep_info)
{
    dinp_stick = win32_info->get_dinp_stick();
}

DIOptIO::~DIOptIO(void)
{
    if ((*dinp_stick))
    {
//		console.sys_message << "OptIO released" << endl;

	(*dinp_stick)->Unacquire();
	(*dinp_stick)->Release();
	(*dinp_stick) = NULL;
    }
}

void DIOptIO::init(void)
{
/* 	hoge << "Initializing OptIOs..." << std::endl;
// enum
if ( FAILED(dinp->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumOptIOsCallback, (LPVOID)((win32_info->get_hwnd())), DIEDFL_ATTACHEDONLY)) )
{
hoge << "Can't Enum Device: OptIO" << std::endl;
(*dinp_stick) = NULL;
}
else if ((*dinp_stick) == NULL) // EnumOptIOCallback‚ÅCreate‚³‚êéÍ¸
{
    hoge << "Can't Get Input Device: OptIO" << std::endl;
    (*dinp_stick) = NULL;
}
else if ( FAILED((*dinp_stick)->SetDataFormat( &c_dfDIOptIO2 )) )
{
    hoge << "Can't Set DataFormat: OptIO" << std::endl;
    (*dinp_stick) = NULL;
}
else if ( FAILED((*dinp_stick)->SetCooperativeLevel( (win32_info->get_hwnd()), DISCL_EXCLUSIVE | DISCL_FOREGROUND )) )
{
    hoge << "Can't Set Cooperativelevel: OptIO" << std::endl;
    (*dinp_stick) = NULL;
}
else if ( FAILED((*dinp_stick)->EnumObjects( EnumObjectsCallback, (VOID*)((win32_info->get_hwnd())), DIDFT_ALL)) )
{
    hoge << "Can't Enum Objects: OptIO" << std::endl;
    (*dinp_stick) = NULL;
} */
}

void DIOptIO::restore(void)
{
    // reget opt_io
    if ((*dinp_stick))			
    {
//		if ((*dinp_stick)->GetDeviceState(sizeof(DIJOYSTATE), &js) == DIERR_INPUTLOST )
	if (FAILED((*dinp_stick)->Acquire()))
	{
//			console.err_message << "Can't Acquire OptIO" << endl;
	}
    }
}

#define	DINP_DOWN	0x01
#define	DINP_LEFT	0x02
#define	DINP_RIGHT	0x04
#define	DINP_UP		0x08
#define	DINP_TRIGA	0x10
#define	DINP_TRIGB	0x20
#define	DINP_TRIGC	0x40
#define	DINP_TRIGD	0x80

Int32 DIOptIO::get_stick(void)
{
    Int32 res = 0;
//    char	kbuf[256];
    DIJOYSTATE	js;

    // check opt_io
    if ((*dinp_stick))
    {
	// poll state
	if ( DI_OK != (*dinp_stick)->Poll() )
	{
//			std::ofstream hemo("stick.txt");
//			hemo << "hemo" << std::endl;
	    if ((*dinp_stick)->Poll() == DIERR_INPUTLOST)
	    {
//				hemo << "DIERR_INPUTLOST" << std::endl;
	    }
	    if ((*dinp_stick)->Poll() == DIERR_NOTACQUIRED )
	    {
//				hemo << "DIERR_NOTACQUIRED" << std::endl;
	    }
	    if ((*dinp_stick)->Poll() == DIERR_NOTINITIALIZED )
	    {
//				hemo << "DIERR_NOTINITIALIZED " << std::endl;
	    }
	    if ((*dinp_stick)->Poll() == DI_NOEFFECT )
	    {
//				hemo << "DI_NOEFFECT " << std::endl;
	    }
//			hemo.close();

	    while ((*dinp_stick)->Acquire() == DIERR_INPUTLOST);
	    return res;
	}

	// get a state
	if ( FAILED((*dinp_stick)->GetDeviceState(sizeof(DIJOYSTATE2), &js)) )
	{
	    return 1;
	}

	// bind of stick
	if (js.lY < -1)
	    res |= DINP_UP;
    	if (js.lY > 1)
	    res |= DINP_DOWN;
	if (js.lX < -1)
	    res |= DINP_LEFT;
    	if (js.lX > 1)
	    res |= DINP_RIGHT;

/*		if (js.rgbButtons[dinp_trigmap.trig_a] & 0x80)
		res |= DINP_TRIGA;
		if (js.rgbButtons[dinp_trigmap.trig_b] & 0x80)
		res |= DINP_TRIGB;
		if (js.rgbButtons[dinp_trigmap.trig_c] & 0x80)
		res |= DINP_TRIGC;
		if (js.rgbButtons[dinp_trigmap.trig_d] & 0x80)
		res |= DINP_TRIGD;
*/	}
    return res;
}

Int32 DIOptIO::get_trigger(void)
{
    return 0;
}

/*
  Input::Input(const HINSTANCE hinst, const HWND hwnd, const BOOL key, const BOOL stick)
  {
  input_initialized = TRUE;

  set_keybind(DIK_W, DIK_S, DIK_A, DIK_D, 
  DIK_N, DIK_J, DIK_K, DIK_L);
  dinp_trigmap.trig_a = 0;
  dinp_trigmap.trig_b = 1;
  dinp_trigmap.trig_c = 2;
  dinp_trigmap.trig_d = 3;

  if ( FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&dinp, NULL)) )
  {
  input_initialized = FALSE;
  }

  if (key)
  {
  console.err_message << "Initializing Keyboard..." << endl;

  if ( FAILED(dinp->CreateDevice(GUID_SysKeyboard, &dinp_key, NULL)))
  {
  input_initialized = FALSE;
  }
  else if ( FAILED(dinp_key->SetDataFormat(&c_dfDIKeyboard)))
  {
  input_initialized = FALSE;
  }
  else if ( FAILED(dinp_key->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)) )
  {
  input_initialized = FALSE;
  }
  }

  if (stick)
  {	
  console.err_message << "Initializing OptIOs..." << endl;
  if ( FAILED(dinp->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumOptIOsCallback, (LPVOID)hwnd, DIEDFL_ATTACHEDONLY)) )
  {
  console.err_message << "Can't Enum Device: OptIO" << endl;
  (*dinp_stick) = NULL;
  }
  else if ((*dinp_stick) == NULL) // EnumOptIOCallback‚ÅCreate‚³‚êéÍ¸
  {
  console.err_message << "Can't Get Input Device: OptIO" << endl;
  (*dinp_stick) = NULL;
  }
  else if ( FAILED((*dinp_stick)->SetDataFormat( &c_dfDIOptIO2 )) )
  {
  console.err_message << "Can't Set DataFormat: OptIO" << endl;
  (*dinp_stick) = NULL;
  }
  else if ( FAILED((*dinp_stick)->SetCooperativeLevel( hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND )) )
  {
  console.err_message << "Can't Set Cooperativelevel: OptIO" << endl;
  (*dinp_stick) = NULL;
  }
  else if ( FAILED((*dinp_stick)->EnumObjects( EnumObjectsCallback, (VOID*)hwnd, DIDFT_ALL)) )
  {
  console.err_message << "Can't Enum Objects: OptIO" << endl;
  (*dinp_stick) = NULL;
  }
  }
  }

  Input::~Input(void)
  {
  if (dinp_key)
  {
  console.sys_message << "Keyboard released" << endl;

  dinp_key->Unacquire();
  dinp_key->Release();
  dinp_key = NULL;
  }

  if ((*dinp_stick))
  {
  console.sys_message << "OptIO released" << endl;

  (*dinp_stick)->Unacquire();
  (*dinp_stick)->Release();
  (*dinp_stick) = NULL;
  }

  if (dinp)
  {
  console.sys_message << "DirectInput device released" << endl;

  dinp->Release();
  dinp = NULL;
  }
  }

  BOOL Input::is_initialized(void)
  {
  return input_initialized;
  }

  void Input::dinp_restore(void)
  {
//	char	kbuf[256];
//	DIJOYSTATE	js;


if (dinp_key)
{
//		if (dinp_key->GetDeviceState(sizeof(kbuf), (void *)&kbuf) == DIERR_INPUTLOST )
if (FAILED(dinp_key->Acquire()))
{
    console.err_message << "Can't Acquire Keyboard" << endl;
}
}

if ((*dinp_stick))			
{
//		if ((*dinp_stick)->GetDeviceState(sizeof(DIJOYSTATE), &js) == DIERR_INPUTLOST )
    if (FAILED((*dinp_stick)->Acquire()))
    {
	console.err_message << "Can't Acquire OptIO" << endl;
    }
}
}

BYTE Input::dinp_get4pad(const BOOL key, const BOOL stick)
{
    BYTE	res = 0;
    char	kbuf[256];
    DIJOYSTATE	js;

    if ((key == TRUE) && (dinp_key))
    {
	if ( FAILED(dinp_key->GetDeviceState(256, (void *)&kbuf)) )
	    return res;

	if (kbuf[dinp_keymap.up] & 0x80)
	    res |= DINP_UP;
	if (kbuf[dinp_keymap.down] & 0x80)
	    res |= DINP_DOWN;
	if (kbuf[dinp_keymap.left] & 0x80)
	    res |= DINP_LEFT;
	if (kbuf[dinp_keymap.right] & 0x80)
	    res |= DINP_RIGHT;
	if (kbuf[dinp_keymap.trig_a] & 0x80)
	    res |= DINP_TRIGA;
	if (kbuf[dinp_keymap.trig_b] & 0x80)
	    res |= DINP_TRIGB;
	if (kbuf[dinp_keymap.trig_c] & 0x80)
	    res |= DINP_TRIGC;
	if (kbuf[dinp_keymap.trig_d] & 0x80)
	    res |= DINP_TRIGD;
    }

    if ((stick == TRUE) && ((*dinp_stick)))
    {
	if ( FAILED((*dinp_stick)->Poll()) )
	{
	    if ((*dinp_stick)->Poll() == DIERR_INPUTLOST)
	    {
		console.err_message << "DIERR_INPUTLOST" << endl;
	    }
	    if ((*dinp_stick)->Poll() == DIERR_NOTACQUIRED )
	    {
		console.err_message << "DIERR_NOTACQUIRED" << endl;
	    }
	    if ((*dinp_stick)->Poll() == DIERR_NOTINITIALIZED )
	    {
		console.err_message << "DIERR_NOTINITIALIZED " << endl;
	    }
	    if ((*dinp_stick)->Poll() == DI_NOEFFECT )
	    {
		console.err_message << "DI_NOEFFECT " << endl;
	    }

	    while ((*dinp_stick)->Acquire() == DIERR_INPUTLOST);
	    return res;
	}

	if ( FAILED((*dinp_stick)->GetDeviceState(sizeof(DIJOYSTATE2), &js)) )
	{
	    return res;
	}

	if (js.lY < -1)
	    res |= DINP_UP;
    	if (js.lY > 1)
	    res |= DINP_DOWN;
	if (js.lX < -1)
	    res |= DINP_LEFT;
    	if (js.lX > 1)
	    res |= DINP_RIGHT;

	if (js.rgbButtons[dinp_trigmap.trig_a] & 0x80)
	    res |= DINP_TRIGA;
	if (js.rgbButtons[dinp_trigmap.trig_b] & 0x80)
	    res |= DINP_TRIGB;
	if (js.rgbButtons[dinp_trigmap.trig_c] & 0x80)
	    res |= DINP_TRIGC;
	if (js.rgbButtons[dinp_trigmap.trig_d] & 0x80)
	    res |= DINP_TRIGD;
    }

    return res;
}

bool Input::set_keybind(DWORD up, DWORD down, DWORD left, DWORD right, 
			DWORD keya, DWORD keyb, DWORD keyc, DWORD keyd)
{
    dinp_keymap.up = up;
    dinp_keymap.down = down;
    dinp_keymap.left = left;
    dinp_keymap.right = right;

    dinp_keymap.trig_a = keya;
    dinp_keymap.trig_b = keyb;
    dinp_keymap.trig_c = keyc;
    dinp_keymap.trig_d = keyd;

    return true;
}

bool Input::set_triggerbind(int a, int b, int c, int d)
{
    dinp_trigmap.trig_a = a;
    dinp_trigmap.trig_b = b;
    dinp_trigmap.trig_c = c;
    dinp_trigmap.trig_d = d;

    return true;
}

*/
