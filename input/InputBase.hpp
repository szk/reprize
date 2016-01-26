#ifndef INPUT_BASE_HPP_
#define INPUT_BASE_HPP_

#include "InputBuffer.hpp"

class EnvDepInfo;

namespace reprize
{
namespace inp
{
#define K_MOD_NONE      0x0000

#define K_MOD_SHIFT     0x0001
#define K_MOD_CONTROL   0x0002
#define K_MOD_ALT       0x0004
#define K_MOD_META      0x0008
#define K_MOD_SUPER     0x0010
#define K_MOD_HYPER     0x0020

#define K_MOD_CAPSLOCK  0x0100
#define K_MOD_SHIFTLOCK 0x0200
#define K_MOD_NUMLOCK   0x0400

#define K_NULL           0
#define K_ESCAPE         1
#define K_0              2
#define K_1              3
#define K_2              4
#define K_3              5
#define K_4              6
#define K_5              7
#define K_6              8
#define K_7              9
#define K_8             10
#define K_9             11
#define K_MINUS         12
#define K_EQUALS        13
#define K_BACK          14
#define K_TAB           15
#define K_Q             16
#define K_W             17
#define K_E             18
#define K_R             19
#define K_T             20
#define K_Y             21
#define K_U             22
#define K_I             23
#define K_O             24
#define K_P             25
#define K_LBRACKET      26
#define K_RBRACKET      27
#define K_ENTER         28
#define K_LCONTROL      29
#define K_A             30
#define K_S             31
#define K_D             32
#define K_F             33
#define K_G             34
#define K_H             35
#define K_J             36
#define K_K             37
#define K_L             38
#define K_SEMICOLON     39
#define K_APOSTROPHE    40
#define K_GRAVE         41
#define K_LSHIFT        42
#define K_BACKSLASH     43
#define K_Z             44
#define K_X             45
#define K_C             46
#define K_V             47
#define K_B             48
#define K_N             49
#define K_M             50
#define K_COMMA         51
#define K_PERIOD        52
#define K_SLASH         53
#define K_RSHIFT        54
#define K_MULTIPLY      55
#define K_LMENU         56
#define K_SPACE         57
#define K_CAPITAL       58
#define K_F1            59
#define K_F2            60
#define K_F3            61
#define K_F4            62
#define K_F5            63
#define K_F6            64
#define K_F7            65
#define K_F8            66
#define K_F9            67
#define K_F10           68
#define K_NUMLOCK       69
#define K_SCROLL        70
#define K_NUMPAD7       71
#define K_NUMPAD8       72
#define K_NUMPAD9       73
#define K_SUBTRACT      74
#define K_NUMPAD4       75
#define K_NUMPAD5       76
#define K_NUMPAD6       77
#define K_ADD           78
#define K_NUMPAD1       79
#define K_NUMPAD2       80
#define K_NUMPAD3       81
#define K_NUMPAD0       82
#define K_DECIMAL       83
#define K_OEM_102       84
#define K_F11           85
#define K_F12           86
#define K_F13           87
#define K_F14           88
#define K_F15           89
#define K_KANA          90
#define K_ABNT_C1       91
#define K_CONVERT       92
#define K_NOCONVERT     93
#define K_YEN           94
#define K_ABNT_C2       95
#define K_NUMPADEQUALS  96
#define K_PREVTRACK     97
#define K_AT            98
#define K_COLON         99
#define K_UNDERLINE    100
#define K_KANJI        101
#define K_STOP         102
#define K_AX           103
#define K_UNLABELED    104
#define K_NEXTTRACK    105
#define K_NUMPADENTER  106
#define K_RCONTROL     107
#define K_MUTE         108
#define K_CALCULATOR   109
#define K_PLAYPAUSE    110
#define K_MEDIASTOP    111
#define K_VOLUMEDOWN   112
#define K_VOLUMEUP     113
#define K_WEBHOME      114
#define K_NUMPADCOMMA  115
#define K_DIVIDE       116
#define K_SYSRQ        117
#define K_RMENU        118
#define K_PAUSE        119
#define K_HOME         120
#define K_UP           121
#define K_PRIOR        122
#define K_LEFT         123
#define K_RIGHT        124
#define K_END          125
#define K_DOWN         126
#define K_NEXT         127
#define K_INSERT       128
#define K_DELETE       129
#define K_LWIN         130
#define K_RWIN         131
#define K_APPS         132
#define K_POWER        133
#define K_SLEEP        134
#define K_WAKE         135
#define K_WEBSEARCH    136
#define K_WEBFAVORITES 137
#define K_WEBREFRESH   138
#define K_WEBSTOP      139
#define K_WEBFORWARD   140
#define K_WEBBACK      141
#define K_MYCOMPUTER   142
#define K_MAIL         143
#define K_MEDIASELECT  144

#define M_BUTTON0      145
#define M_BUTTON1      146
#define M_BUTTON2      147
#define M_BUTTON3      148
#define M_BUTTON4      149
#define M_BUTTON5      150
#define M_BUTTON6      151
#define M_BUTTON7      152

#define INPUT_MAX_CODE 153

class Keyboard;
class Mouse;
class OptIO;

class InputBase
{
public:
    InputBase(EnvDepInfo* depinfo_) {}
    virtual ~InputBase(void) {}

    virtual const bool init(void)
        {
            const Str keycode_v[]
                = { "K_NULL", "K_ESCAPE",
                    "K_1", "K_2", "K_3", "K_4", "K_5", "K_6", "K_7", "K_8",
                    "K_9", "K_0", "K_MINUS", "K_EQUALS", "K_BACK", "K_TAB",
                    "K_Q", "K_W", "K_E", "K_R", "K_T", "K_Y", "K_U", "K_I",
                    "K_O", "K_P", "K_LBRACKET", "K_RBRACKET", "K_ENTER",
                    "K_LCONTROL", "K_A", "K_S", "K_D", "K_F", "K_G", "K_H",
                    "K_J", "K_K", "K_L", "K_SEMICOLON", "K_APOSTROPHE",
                    "K_GRAVE", "K_LSHIFT", "K_BACKSLASH", "K_Z", "K_X", "K_C",
                    "K_V", "K_B", "K_N", "K_M", "K_COMMA", "K_PERIOD",
                    "K_SLASH", "K_RSHIFT", "K_MULTIPLY", "K_LMENU", "K_SPACE",
                    "K_CAPITAL", "K_F1", "K_F2", "K_F3", "K_F4", "K_F5", "K_F6",
                    "K_F7", "K_F8", "K_F9", "K_F10", "K_NUMLOCK", "K_SCROLL",
                    "K_NUMPAD7", "K_NUMPAD8", "K_NUMPAD9", "K_SUBTRACT",
                    "K_NUMPAD4", "K_NUMPAD5", "K_NUMPAD6", "K_ADD", "K_NUMPAD1",
                    "K_NUMPAD2", "K_NUMPAD3", "K_NUMPAD0", "K_DECIMAL",
                    "K_OEM_102", "K_F11", "K_F12", "K_F13", "K_F14", "K_F15",
                    "K_KANA", "K_ABNT_C1", "K_CONVERT", "K_NOCONVERT", "K_YEN",
                    "K_ABNT_C2", "K_NUMPADEQUALS", "K_PREVTRACK", "K_AT",
                    "K_COLON", "K_UNDERLINE", "K_KANJI", "K_STOP", "K_AX",
                    "K_UNLABELED", "K_NEXTTRACK", "K_NUMPADENTER", "K_RCONTROL",
                    "K_MUTE", "K_CALCULATOR", "K_PLAYPAUSE", "K_MEDIASTOP",
                    "K_VOLUMEDOWN", "K_VOLUMEUP", "K_WEBHOME", "K_NUMPADCOMMA",
                    "K_DIVIDE", "K_SYSRQ", "K_RMENU", "K_PAUSE", "K_HOME",
                    "K_UP", "K_PRIOR", "K_LEFT", "K_RIGHT", "K_END", "K_DOWN",
                    "K_NEXT", "K_INSERT", "K_DELETE", "K_LWIN", "K_RWIN",
                    "K_APPS", "K_POWER", "K_SLEEP", "K_WAKE", "K_WEBSEARCH",
                    "K_WEBFAVORITES", "K_WEBREFRESH", "K_WEBSTOP",
                    "K_WEBFORWARD", "K_WEBBACK", "K_MYCOMPUTER", "K_MAIL",
                    "K_MEDIASELECT", "M_BUTTON0", "M_BUTTON1", "M_BUTTON2",
                    "M_BUTTON3", "M_BUTTON4", "M_BUTTON5", "M_BUTTON6",
                    "M_BUTTON7", };

            const Str keyname_v[]
                = { "null", "ESC",
                    "1", "2", "3", "4", "5", "6", "7", "8",
                    "9", "0", "-", "=", "Backspace", "Tab",
                    "q", "w", "e", "r", "t", "y", "u", "i",
                    "o", "p", "[", "]", "Enter",
                    "Left Control", "a", "s", "d", "f", "g", "h",
                    "j", "k", "l", ";", "'", "`",
                    "Left Shift", "\\", "z", "x", "c",
                    "v", "b", "n", "m", ",", ".",
                    "/", "Right Shift", "*", "Left Alt", "Space",
                    "Capslock", "F1", "F2", "F3", "F4", "F5", "F6",
                    "F7", "F8", "F9", "F10", "Numlock", "Scroll lock",
                    "Numpad 7", "Numpad 8", "Numpad 9", "Numpad -",
                    "Numpad 4", "Numpad 5", "Numpad 6", "Numpad +", "Numpad 1",
                    "Numpad 2", "Numpad 3", "Numpad 0", "Numpad .",
                    "102", "F11", "F12", "F13", "F14", "F15",
                    "Kana", "C1", "Henkan", "Muhenkan", "\"",
                    "C2", "Numpad =", "Prev Track", "@",
                    ":", "_", "Kanji", "Stop", "AX",
                    "Unlabeled", "Next Track", "Numpad Enter", "Right Control",
                    "Mute", "Calculator", "Playpause", "Mediastop",
                    "Volumedown", "Volumeup", "Webhome", "Numpad ,",
                    "Numpad /", "SysRQ", "Right Alt", "Pause", "Home",
                    "Up", "Pageup", "Left", "Right", "End", "Down",
                    "Pagedown", "Insert", "Delete", "Left Win", "Right Win",
                    "Apps", "Power", "Sleep", "Wakeup", "Websearch",
                    "Webfavorites", "Webrefresh", "Webstop",
                    "Webforward", "Webback", "My Computer", "Mail",
                    "Mediaselect", "Button0", "Button1", "Button2",
                    "Button3", "Button4", "Button5", "Button6",
                    "Button7", };

            for (InputCode i = 0; INPUT_MAX_CODE > i; ++i)
            {
                keycodes.insert(make_pair(keycode_v[i], i));
                keyname_str[i].assign(keyname_v[i]);
            }

            return true;
        }
    virtual void restore(void) {}
    virtual void check(Keyboard*, Mouse*, OptIO*) {}
    virtual void reset(void) { pressed.reset(); released.reset(); }

    const Str& code_to_name(const InputCode code_)
        {
            if (INPUT_MAX_CODE > code_) { return keyname_str[code_]; }
            return keyname_str[K_NULL];
        }

    InputCode str_to_code(const Str& key_name_)
        {
            if (keycodes.find(key_name_) == keycodes.end())
            { return K_NULL; }
            return keycodes.find(key_name_)->second;
        }

    InputBuffer& get_pressed(void) { return pressed; }
    InputBuffer& get_released(void) { return released; }

protected:
    InputBuffer pressed, released;
    Str keyname_str[INPUT_MAX_CODE];
    std::map<const Str, InputCode> keycodes;
};
}
}
#endif
