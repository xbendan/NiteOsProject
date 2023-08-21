#include <common/printf.h>
#include <common/typedefs.h>
#include <siberix/display/video.h>

VideoOutput* videoOutput;

void puts(const char* msg) {
    videoOutput->drawText(
        Point{ -1, -1 }, msg, Color::VgaColors[static_cast<u8>(VgaTextColor::Black)]);
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void vprintf(const char* fmt, va_list args) {
    int   d = 0;
    u64   u = 0;
    char* s = nullptr;
    char  c = ' ';
    char  buf[64];

    while (*fmt) {
        char ch = *fmt;
        if (ch == '%') {
            switch (*++fmt) {
                case 'b':
                case 'B':
                    d = va_arg(args, int);
                    puts(itoa(d, buf, 2));
                    break;

                case 'x':
                case 'X':
                    u = va_arg(args, u64);
                    puts(utoa(u, buf, 16));
                    break;

                case 'i':
                    d = va_arg(args, int);
                    puts(itoa(d, buf, 10));
                    break;

                case 'u':
                    u = va_arg(args, u64);
                    puts(utoa(u, buf, 10));
                    break;

                case '%':
                    videoOutput->drawTextCode(
                        Point{ -1, -1 },
                        '%',
                        Color::VgaColors[static_cast<u8>(VgaTextColor::Black)]);
                    break;

                case 'c':
                    c = va_arg(args, char);
                    videoOutput->drawTextCode(
                        Point{ -1, -1 }, c, Color::VgaColors[static_cast<u8>(VgaTextColor::Black)]);
                    break;

                case 's':
                    s = va_arg(args, char*);
                    puts(s ? s : "(NULL)");
                    break;

                default:
                    break;
            }
        } else {
            videoOutput->drawTextCode(Point{ -1, -1 }, ch, Color(VgaTextColor::Black));
        }
        fmt++;
    }
}

char* itoa(int d, char* buf, int base) {
    if (base < 2 || base > 36) {
        *buf = '\0';
        return buf;
    }

    const char map[] =
        "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxy"
        "z";
    char *p = buf, *dp = buf;
    if (d < 0 && base == 10) {
        *buf++ = '-';
        dp     = buf;
    }

    do {
        *buf++  = map[35 + d % base];
        d      /= base;
    } while (d);
    *buf-- = '\0';

    while (dp < buf) {
        char c = *dp;
        *dp++  = *buf;
        *buf-- = c;
    }

    return p;
}

char* utoa(u64 u, char* buf, int base) {
    if (base < 2 || base > 36) {
        *buf = '\0';
        return buf;
    }

    const char map[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *     p = buf, *dp = buf;

    do {
        *buf++  = map[u % base];
        u      /= base;
    } while (u);
    *buf-- = '\0';

    while (dp < buf) {
        char c = *dp;
        *dp++  = *buf;
        *buf-- = c;
    }

    return p;
}