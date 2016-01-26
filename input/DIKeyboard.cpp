#include "Common.hpp"
#include "Win32DepInfo.hpp"

#include "DIKeyboard.hpp"

using namespace reprize;

DIKeyboard::DIKeyboard(Win32DepInfo* dep_info)
    : Keyboard(dep_info), win32_info(dep_info)
{
    dinp_key = win32_info->get_dinp_key();
}

DIKeyboard::~DIKeyboard(void)
{
    if (dinp_key)
    {
//		console.sys_message << "Keyboard released" << endl;

	(*dinp_key)->Unacquire();
	(*dinp_key)->Release();
	(*dinp_key) = NULL;
    }
}

void DIKeyboard::init(void)
{
}

void DIKeyboard::restore(void)
{
    // reget keyboard
    if (dinp_key)
    {
//		if (dinp_key->GetDeviceState(sizeof(kbuf), (void *)&kbuf) == DIERR_INPUTLOST )
	if (FAILED((*dinp_key)->Acquire()))
	{
//			console.err_message << "Can't Acquire Keyboard" << endl;
	}
    }
}

Int32 DIKeyboard::get_key(void)
{
    BYTE	res = 0;
    char	kbuf[256];

    // check key
    if (dinp_key)
    {
	// get state
	if ( FAILED((*dinp_key)->GetDeviceState(256, (void *)&kbuf)) )
	    return res;

	// bind cursor
	if (kbuf[DIK_Q] & 0x80)
	    return DIK_Q;
	if (kbuf[DIK_E] & 0x80)
	    return DIK_E;
	if (kbuf[DIK_W] & 0x80)
	    return DIK_W;
	if (kbuf[DIK_S] & 0x80)
	    return DIK_S;
	if (kbuf[DIK_A] & 0x80)
	    return DIK_A;
	if (kbuf[DIK_D] & 0x80)
	    return DIK_D;
    }

    return 0;
}
