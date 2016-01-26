#include "Common.hpp"

#include "misc/X11DepInfo.hpp"
#include "XEvKeyboard.hpp"

using namespace reprize;
using namespace inp;
using namespace std;

KeySym keysym_idx[] = {
    0, XK_Escape,
    XK_0, XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_8, XK_9,
    XK_minus, XK_equal, XK_BackSpace, XK_Tab,
    XK_Q, XK_W, XK_E, XK_R, XK_T, XK_Y, XK_U, XK_I, XK_O, XK_P,
    XK_bracketleft, XK_bracketright, XK_Return, XK_Control_L,
    XK_A, XK_S, XK_D, XK_F, XK_G, XK_H, XK_J, XK_K, XK_L,
    XK_semicolon, XK_apostrophe, XK_grave, XK_Shift_L, XK_backslash,
    XK_Z, XK_X, XK_C, XK_V, XK_B, XK_N, XK_M, XK_comma, XK_period, XK_slash,
    XK_Shift_R, XK_multiply, XK_Meta_L, XK_space, XK_Caps_Lock,
    XK_F1, XK_F2, XK_F3, XK_F4, XK_F5, XK_F6, XK_F7, XK_F8, XK_F9, XK_F10,
    XK_Num_Lock, XK_Scroll_Lock,
    XK_KP_7, XK_KP_8, XK_KP_9, XK_KP_Subtract,
    XK_KP_4, XK_KP_5, XK_KP_6, XK_KP_Add,
    XK_KP_1, XK_KP_2, XK_KP_3, XK_KP_0, XK_KP_Decimal, XK_VoidSymbol,
    XK_F11, XK_F12, XK_F13, XK_F14, XK_F15,
    XK_Kana_Lock, XK_VoidSymbol, XK_Henkan, XK_Muhenkan, XK_yen, XK_VoidSymbol,
    XK_KP_Equal, XK_VoidSymbol, XK_at, XK_colon, XK_underscore, XK_Kanji,
    XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol, XK_KP_Enter,
    XK_Control_R, XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol,
    XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol, XK_KP_Decimal, XK_KP_Divide,
    XK_Sys_Req, XK_Meta_R, XK_Pause, XK_Home,
    XK_Up, XK_Prior, XK_Left, XK_Right, XK_End, XK_Down, XK_Next,
    XK_Insert, XK_Delete,
    XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol,
    XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol,
    XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol, XK_VoidSymbol
};

XEvKeyboard::XEvKeyboard(X11DepInfo* dep_info_)
    : Keyboard(dep_info_), x11_info(dep_info_)
{
    RE_ZERO(key_buf, K_BUF_SIZE);

    for (Size32 i = 0; i < INPUT_MAX_CODE; ++i)
    {
        x_keycode_map.insert(pair<KeyCode, InputCode>
                             (XKeysymToKeycode(x11_info->get_dpy(),
                                               keysym_idx[i]), i));
    }
}

XEvKeyboard::~XEvKeyboard(void)
{
}

const bool XEvKeyboard::init(void)
{
    repeat = false;
    post_repeat = false;
    Keyboard::init();

    return true;
}

// see FocusIn/Out
void XEvKeyboard::restore(void)
{
//    XRefreshKeyboardMapping(&(x11_info->get_event()->xmapping););
}

void XEvKeyboard::reset(void)
{
    // strings
    strbuf[0] =  NULCHAR;
    strbuf_len = 0;
    // keys
    mod = K_MOD_NONE;
}

void XEvKeyboard::check(InputBuffer& pressed_, InputBuffer& released_)
{
    if (x11_info->get_event()->xkey.state & ShiftMask)
    { mod |= K_MOD_SHIFT; }
    if (x11_info->get_event()->xkey.state & ControlMask)
    { mod |= K_MOD_CONTROL; }
    if (x11_info->get_event()->xkey.state & meta_mask)
    { mod |= K_MOD_META; }
    if (x11_info->get_event()->xkey.state & numlock_mask)
    { mod |= K_MOD_NUMLOCK; }

    for (Size32 i = 0; i < 32; ++i) { post_kstate[i] = kstate[i]; }

    XQueryKeymap(x11_info->get_dpy(), kstate);

    for (Size32 i = 0; i < 32; ++i)
    {
        if (post_kstate[i] == kstate[i]) { continue; }
        for (Size32 j = 0; j < 8; ++j)
        {
            if ((kstate[i] & 1 << j) && !(post_kstate[i] & 1 << j))
            { add_key(translate_keycode(i * 8 + j), pressed_); }
            if (!(kstate[i] & 1 << j) && (post_kstate[i] & 1 << j))
            { add_key(translate_keycode(i * 8 + j), released_); }
        }
    }

    if (x11_info->get_event()->type == KeyPress)
    {
        KeySym keysym;
        if (XLookupString(&(x11_info->get_event()->xkey),
                          (char *)key_buf, K_BUF_SIZE, &keysym, NULL) == 0)
        { return; }

        if (keysym >= 0x0100 && keysym < 0x0800)
        {
            // special keys XXX
            key_buf[0] = (keysym & 0xFF);
            key_buf[1] = NULCHAR;
        }
        else
        {
            // normal keys
            strbuf[strbuf_len] = key_buf[0];
            strbuf[++strbuf_len] = NULCHAR;
            key_buf[0] = NULCHAR;
        }
    }
}

InputCode XEvKeyboard::translate_keycode(KeyCode x_keycode_)
{
    map<KeyCode, InputCode>::iterator itr = x_keycode_map.find(x_keycode_);

    if (itr != x_keycode_map.end()) { return itr->second; }
    return INPUT_MAX_CODE;
}
