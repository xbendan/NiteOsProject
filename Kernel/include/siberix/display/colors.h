#include <common/typedefs.h>

enum class VgaTextColor {
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

static Color VgaColors[16] = { Color(0, 0, 0),      Color(0, 0, 170),
                               Color(0, 170, 0),    Color(0, 170, 170),
                               Color(170, 0, 0),    Color(170, 0, 170),
                               Color(170, 85, 0),   Color(170, 170, 170),
                               Color(85, 85, 85),   Color(85, 85, 255),
                               Color(85, 255, 85),  Color(85, 255, 255),
                               Color(255, 85, 85),  Color(255, 85, 255),
                               Color(255, 255, 85), Color(255, 255, 255) };

class Color {
public:
    Color(u32 rgba);
    Color(u8 r, u8 g, u8 b, u8 a);
    Color(u8 r, u8 g, u8 b);
    Color(u8 vga);
    ~Color() = default;

    bool operator==(Color& color) {
        return (color.r == r) && (color.g == g) && (color.b == b) &&
               (color.a == a);
    }

    inline u8 asVgaIndex(u8 fallback = 0) {
        u8 n = 0;
        while (n++ < 16) {
            if (VgaColors[n] == 0) {
                return n;
            }
        }
        return fallback;
    }

    inline u32 asHexCode(bool withAlpha) {}

    static inline u8 getVgaColorIndex(Color& color, u8 fallback) {
        u8 n = 0;
        while (n++ < 16) {
            if (VgaColors[n] == color) {
                return n;
            }
        }
        return fallback;
    }

    u8 r, g, b, a;
};