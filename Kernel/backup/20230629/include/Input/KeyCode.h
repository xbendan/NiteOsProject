#pragma once

#include <macros>

namespace Input
{
    enum Key
    {
        None,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        PageScroll, ScrollLock, PauseBreak, NumberLock,
        Insert, Delete, Home, End, PageUp, PageDown,
        Escape, Tab, CapsLock, Space, Backspace, Enter,
        LShift, LCtrl, LAlt, RAlt, RCtrl, RShift,
        ArrowLeft, ArrowRight, ArrowUp, ArrowDown,
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
        A, B, C, D, E, F, G, H, I, J, K, L, M, 
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, 
        Numpad6, Numpad7, Numpad8, Numpad9, Numpad0,
        NumpadAdd, NumpadMinus, NumpadEnter, NumpadSlash, NumpadMultiply, NumpadPeriod,
        Exclamation /* ! */, DoubleQuote /* " */, Pound /* # */, Dollar /* $ */, 
        Percentage /* % */, And /* & */, Quote /* ' */, 
        LeftBracket /* ( */, RightBracket /* ) */, 
        Multiply /* * */, Add /* + */, Comma /* , */, Minus /* - */, 
        Period /* . */, Slash /* / */, Backtick /* ` */,
        Equals, Backslash, Colon, Semicolon, LessThan, GreaterThan, 
        Question, At, Caret, Underline, Wave,
        LeftBrace, RightBrace, LeftMenu, RightMenu,
        PreviousTrack, NextTrack, Mute, MediaPlay, MediaStop, MediaSelect,
        VolumeDown, VolumeUp, Calculator, Apps, Email, Computer,
        CursorLeft, CursorRight, CursorUp, CursorDown,
        WebHome, WebRefresh, WebStop, WebSearch, WebForward, WebBackward, WebFavorites,
        Power, Sleep, Wake
    };

    enum KeyState
    {
        KeyPressed = 0,
        KeyReleased = 0
    };

    enum KeyModifiers
    {
        KeyModifiersAlt = 0x01,
        KeyModifiersShift = 0x02,
        KeyModifiersCtrl = 0x04,
        KeyModifiersCapsLock = 0x08
    };

    extern Key c_KeyScanCode1[2][128];

    char ApplyModifiers(Key key, uint8_t modifiers);

    // inline char KeyToChar(Key key)
    // {
    //     if (key >= A && key <= Z)
    //         return 'A' + (key - A);
    //     else if (key >= Num0 && key <= Num9)
    //         return '0' + (key - 39);
    //     else if (key >= 91 && key <= 105)
    //         return '!' + (key - 91);
    //     else 
    //         switch (key)
    //         {
    //         // case Exclamation: return '!';
    //         // case DoubleQuote: return '\"';
    //         // case Pound: return '#';
    //         // case Dollar: return '$';
    //         // case Percentage: return '%';
    //         // case And: return '&';
    //         // case Quote: return '\'';
    //         // case LeftBracket: return '(';
    //         // case RightBracket: return ')';
    //         // case Multiply: return '*';
    //         // case Add: return '+';
    //         // case Comma: return ',';
    //         // case Minus: return '-';
    //         // case Period: return '.';
    //         // case Slash: return '/';
    //         case Space: return ' ';
    //         case Colon: return ':';
    //         case Semicolon: return ';';
    //         case LessThan: return '<';
    //         case Equals: return '=';
    //         case GreaterThan: return '>';
    //         case Question: return '?';
    //         case At: '@';
    //         case LeftBracket: '[';
    //         case Backslash: '\\';
    //         case RightBracket: ']';
    //         case Caret: '^';
    //         case Underline: '_';
    //         case Backtick: '`';
    //         case Wave: '~';
            
    //         default:
    //             break;
    //         }
    // };
}