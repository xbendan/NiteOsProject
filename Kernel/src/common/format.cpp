#include <common/format.h>
#include <common/string.h>

extern "C" {

void strfmt(char* dest, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    strfmt(dest, fmt, args);
    va_end(args);
}

void strfmts(char* dest, const char* fmt, va_list args) {
    int           d   = 0;
    unsigned long u   = 0;
    char*         s   = nullptr;
    char          c   = ' ';
    int           len = 0;

    char buf[64];
    int  i = 0;

    while (*fmt) {
        char ch = *fmt;
        if (ch == '%') {
            switch (*++fmt) {
                case 'b':
                    d   = va_arg(args, int);
                    s   = itoa(d, buf, 2);
                    len = strlen(s);
                    memcpy(&(dest[i]), s, len);
                    i += len;
                    break;
                case 'x':
                    u   = va_arg(args, unsigned long);
                    s   = utoa(u, buf, 16);
                    len = strlen(s);
                    memcpy(&(dest[i]), s, len);
                    i += len;
                    break;
                case 'i':
                    d   = va_arg(args, int);
                    s   = itoa(d, buf, 10);
                    len = strlen(s);
                    memcpy(&(dest[i]), s, len);
                    i += len;
                    break;
                case 'u':
                    u   = va_arg(args, unsigned long);
                    s   = utoa(u, buf, 10);
                    len = strlen(s);
                    memcpy(&(dest[i]), s, len);
                    i += len;
                    break;
                case '%':
                    dest[i++] = '%';
                    break;
                case 'c':
                    c         = va_arg(args, int);
                    dest[i++] = c;
                    break;
                case 's':
                    s   = va_arg(args, char*);
                    len = strlen(s);
                    memcmp(&(dest[i]), s, len);
                    i += len;
                    break;
                default:
                    break;
            }
        } else {
            dest[i++] = ch;
        }
        fmt++;
    }
    dest[i] = '\0';
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

char* utoa(unsigned long u, char* buf, int base) {
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
}