#pragma once

#include <common/typedefs.h>

enum class VgaTextColor : u8 {
    Black,
    Blue,
    Green,
    Cyan,
    Red,
    Purple,
    Brown,
    Gray,
    DarkGray,
    LightBlue,
    LightGreen,
    LightCyan,
    LightRed,
    LightPurple,
    Yellow,
    White
};

class Color {
public:
    Color(u32 rgba) {
        r = (rgba >> 24) & 0xff;
        g = (rgba >> 16) & 0xff;
        b = (rgba >> 8) & 0xff;
        a = rgba & 0xff;
    }
    Color(u8 _r, u8 _g, u8 _b, u8 _a)
        : r(_r),
          g(_g),
          b(_b),
          a(_a) {}
    Color(u8 _r, u8 _g, u8 _b)
        : r(_r),
          g(_g),
          b(_b),
          a(0xff) {}
    Color(VgaTextColor vga) { *this = VgaColors[static_cast<u8>(vga)]; }
    ~Color() = default;

    bool operator==(Color& color) {
        return (color.r == r) && (color.g == g) && (color.b == b) && (color.a == a);
    }

    inline u8 asVgaIndex(VgaTextColor fallback = VgaTextColor::Black) {
        u8 n = 0;
        while (n++ < 16) {
            if (VgaColors[n] == *this) {
                return n;
            }
        }
        return static_cast<u8>(fallback);
    }

    inline u32 asHexCode(bool withAlpha) {
        u32 code = (r << 16) | (g << 8) | b;
        if (withAlpha) {
            code |= a << 24;
        }
        return code;
    }

    static inline u8 getVgaColorIndex(Color& color, VgaTextColor fallback) {
        u8 n = 0;
        while (n++ < 16) {
            if (VgaColors[n] == color) {
                return n;
            }
        }
        return static_cast<u8>(fallback);
    }

    static Color VgaColors[16];

    u8 r, g, b, a;
};