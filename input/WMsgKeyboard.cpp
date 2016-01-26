#include "Common.hpp"

#include "WMsgKeyboard.hpp"

#include <WinUser.h>

using namespace reprize;
using namespace inp;
using namespace std;

UINT vk_idx[] = {
    0, VK_ESCAPE,
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    VK_OEM_MINUS, VK_OEM_PLUS, VK_BACK, VK_TAB,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
    VK_OEM_4, VK_OEM_6, VK_RETURN, VK_LCONTROL,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
    VK_OEM_1, VK_OEM_7, VK_OEM_3, VK_LSHIFT, VK_OEM_5,
    'Z', 'X', 'C', 'V', 'B', 'N', 'M',
    VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_2,
    VK_RSHIFT, VK_MULTIPLY, VK_LMENU, VK_SPACE, VK_CAPITAL,
    VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10,
    VK_NUMLOCK, VK_SCROLL,
    VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9, VK_SUBTRACT,
    VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_ADD,
    VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD0, VK_DECIMAL, VK_OEM_102,
    VK_F11, VK_F12, VK_F13, VK_F14, VK_F15,
    VK_KANA, VK_NONAME, VK_CONVERT, VK_NONCONVERT, VK_NONAME, VK_NONAME,
    VK_NONAME, VK_NONAME, VK_NONAME, VK_NONAME, VK_NONAME, VK_NONAME,
    VK_NONAME, VK_OEM_AX, VK_NONAME, VK_NONAME, VK_NONAME,
    VK_RCONTROL, VK_NONAME, VK_NONAME, VK_NONAME, VK_NONAME,
    VK_NONAME, VK_NONAME, VK_NONAME, VK_NONAME, VK_DIVIDE,
    VK_NONAME, VK_RMENU, VK_PAUSE, VK_HOME,
    VK_UP, VK_PRIOR, VK_LEFT, VK_RIGHT, VK_END, VK_DOWN, VK_NEXT,
    VK_INSERT, VK_DELETE,
    VK_LWIN, VK_RWIN, VK_APPS, VK_NONAME, VK_SLEEP,
    VK_NONAME, VK_NONAME, VK_NONAME, VK_NONAME, VK_NONAME,
    VK_NONAME, VK_NONAME, VK_NONAME, VK_NONAME, VK_NONAME, 
};

WMsgKeyboard::WMsgKeyboard(Win32DepInfo* dep_info)
    : Keyboard(dep_info), win32_info(dep_info)
{
    for (InputCode i = 0; i < INPUT_MAX_CODE; ++i)
    { win32_keycodes.insert(make_pair(vk_idx[i], i)); }
}

WMsgKeyboard::~WMsgKeyboard(void)
{
}

const bool WMsgKeyboard::init(void)
{
   Keyboard::init();
   GetKeyboardState(post_kstate);

   return true;
}

// see FocusIn/Out
void WMsgKeyboard::restore(void)
{
}

void WMsgKeyboard::reset(void)
{
    // strings
    strbuf[0] = NULCHAR;
    strbuf_len = 0;
    // keys
    mod = K_MOD_NONE;
}

void WMsgKeyboard::check(InputBuffer& pressed_, InputBuffer& released_)
{
    GetKeyboardState(kstate);

    for (Size32 i = 0; i < 256; ++i)
    {
        if ((kstate[i] & 0x80) && !(post_kstate[i] & 0x80))
        { add_key(translate_keycode(static_cast<UINT>(i)), pressed_); }
        else if (!(kstate[i] & 0x80) && (post_kstate[i] & 0x80))
        { add_key(translate_keycode(static_cast<UINT>(i)), released_); }

        post_kstate[i] = kstate[i];
    }

    if (win32_info->get_msg()->message == WM_CHAR)
    {
        if (STR_BUF_SIZE > strbuf_len)
        {
            strbuf[strbuf_len] = static_cast<uChar>
                (win32_info->get_msg()->wParam);
            strbuf[++strbuf_len] = NULCHAR;
        }
    }

#ifdef USE_IM
    // check here http://home.catv.ne.jp/pp/ginoue/im/win32.html
    if (win32_info->get_msg()->message == WM_IME_COMPOSITION)
    {
        HIMC himc = ImmGetContext(win32_info->get_hwnd());
        if (win32_info->get_msg()->lParam & GCS_RESULTSTR)
        {
            len = ImmGetCompositionString(himc, GCS_RESULTSTR, NULL, 0);
            ImmGetCompositionString(himc, GCS_RESULTSTR, str, len);
            ImmReleaseContext(win32_info->get_hwnd(), himc);
            str[len] = '\0';
        }
    }
#endif
}

InputCode WMsgKeyboard::translate_keycode(UINT win32_keycode_)
{
    map<UINT, InputCode>::iterator itr = win32_keycodes.find(win32_keycode_);

    if (itr != win32_keycodes.end()) { return itr->second; }
    return INPUT_MAX_CODE;
}
