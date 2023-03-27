#include <Drivers/Input/KeyCode.h>

namespace Input
{
    Key c_KeyScanCode1[128] =
    {
        None, // 0x00, 0
        Escape, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0, Minus, Equals, Backspace, // 0x0E, 14
        Tab, Q, W, E, R, T, Y, U, I, O, P, LeftBrace, RightBrace, Enter, // 0x1C, 28
        LCtrl, A, S, D, F, G, H, J, K, L, Semicolon, Quote, Backtick, // 0x29, 41
        LShift, Backslash, Z, X, C, V, B, N, M, Comma, Period, Slash, RShift, // 0x36, 54
        NumpadMultiply, LAlt, Space, CapsLock, // 0x3A, 58
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, // 0x44, 68
        NumberLock, ScrollLock, // 0x46, 70
        Numpad7, Numpad8, Numpad9, NumpadMinus, // 0x4A, 74
        Numpad4, Numpad5, Numpad6, NumpadAdd, // 0x4E, 78
        Numpad1, Numpad2, Numpad3, // 0x51, 81
        Numpad0, NumpadPeriod, // 0x53, 83,
        None,
        None,
        None,
        F11, // 0x57, 87
        F12, // 0x58, 88
    };

    char ApplyModifiers(Key key, KeyModifiers modifiers)
    {
        if (key >= A && key <= Z)
        {
            bool capslock = (modifiers & KeyModifiersCapsLock);
            if (modifiers & KeyModifiersShift)
                capslock = !capslock;

            uint8_t base = capslock ? 'A' : 'a';
            return base + (key - A);
        }
        else if (key >= Num0 && key <= Num9)
        {
            if (modifiers & KeyModifiersShift)
                switch (key)
                {
                case Num1: return '!';
                case Num2: return '@';
                case Num3: return '#';
                case Num4: return '$';
                case Num5: return '%';
                case Num6: return '^';
                case Num7: return '&';
                case Num8: return '*';
                case Num9: return '(';
                case Num0: return ')';
                default: return 0x00;
                }
            else
                return '0' + (key - Num0);
        }
        else
        {
            if (modifiers & KeyModifiersShift)
                switch (key)
                {
                case Backtick: return '`';
                case Minus: return '-';
                case Equals: return '=';
                case LeftBracket: return '[';
                case RightBracket: return ']';
                case Backslash: return '\\';
                case Semicolon: return ';';
                case Quote: return '\'';
                case Comma: return ',';
                case Period: return '.';
                case Slash: return '/';
                default: return 0x00;
                }
            else
                switch (key)
                {
                case Backtick: return '~';
                case Minus: return '_';
                case Equals: return '+';
                case LeftBracket: return '{';
                case RightBracket: return '}';
                case Backslash: return '|';
                case Semicolon: return ':';
                case Quote: return '\"';
                case Comma: return '<';
                case Period: return '>';
                case Slash: return '?';
                default: return 0x00;
                }
        }
    }
} // namespace Input
