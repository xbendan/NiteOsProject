#include <libkern/printf.h>
#include <libkern/sprintf.h>
#include <macros>
#include <drv/video.h>

void puts(const char *msg) {
    const char *p = msg;
    while (*p) {
        Video::WriteChar(*p);
        p++;
    }
}

void printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void vprintf(const char *fmt, va_list args) {
    int d = 0;
    uint64_t u = 0;
    char* s = nullptr;
    char c = ' ';
    char buf[64];

    while (*fmt) {
        char ch = *fmt;
        if (ch == '%') {
            switch(*++fmt) {
                case 'b': case 'B':
                    d = va_arg(args, int);
                    puts(itoa(d, buf, 2));
                    break;

                case 'x': case 'X':
                    u = va_arg(args, uint64_t);
                    puts(utoa(u, buf, 16));
                    break;

                case 'i': 
                    d = va_arg(args, int);
                    puts(itoa(d, buf, 10));
                    break;

                case 'u':
                    u = va_arg(args, uint64_t);
                    puts(utoa(u, buf, 10));
                    break;
                    
                case '%':
                    Video::WriteChar('%');
                    break;

                case 'c':
                    c = va_arg(args, char);
                    Video::WriteChar(c);
                    break;

                case 's':
                    s = va_arg(args, char*);
                    puts(s?s:"(NULL)");
                    break;

                default:
                    break;
            }
        } else {
            Video::WriteChar(ch);
        }
        fmt++;
    }
}