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
    Color(u32 rgba);
    Color(u8 r, u8 g, u8 b, u8 a);
    Color(u8 r, u8 g, u8 b);
    Color(u8 vga);
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

    inline u32 asHexCode(bool withAlpha) {}

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