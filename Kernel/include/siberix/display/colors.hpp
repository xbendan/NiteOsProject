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

    static inline u8 getVgaColorIndex(Color& color, u8 fallback) {
        u8 n = 0;
        while (n++ < 16) {
            if (VGA_COLORS[n] == color) {
                return n;
            }
        }
        return fallback;
    }

    u8 r, g, b, a;

    static inline Color VGA_COLORS[] = [
        [0]  = Color(0, 0, 0),
        [1]  = Color(0, 0, 170),
        [2]  = Color(0, 170, 0),
        [3]  = Color(0, 170, 170),
        [4]  = Color(170, 0, 0),
        [5]  = Color(170, 0, 170),
        [6]  = Color(170, 85, 0),
        [7]  = Color(170, 170, 170),
        [8]  = Color(85, 85, 85),
        [9]  = Color(85, 85, 255),
        [10] = Color(85, 255, 85),
        [11] = Color(85, 255, 255),
        [12] = Color(255, 85, 85),
        [13] = Color(255, 85, 255),
        [14] = Color(255, 255, 85),
        [15] = Color(255, 255, 255)
    ];
};