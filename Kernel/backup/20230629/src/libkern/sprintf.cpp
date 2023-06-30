#include <libkern/sprintf.h>
#include <macros>

char* itoa(int d, char* buf, int base)
{
    if (base < 2 || base > 36) {
        *buf = '\0';
        return buf;
    }

    const char map[] = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";
    char* p = buf, *dp = buf;
    if (d < 0 && base == 10) {
        *buf++ = '-';
        dp = buf;
    }

     do {
         *buf++ = map[35 + d % base];
         d /= base;
     } while (d);
     *buf-- = '\0';

     while (dp < buf) {
         char c = *dp;
         *dp++ = *buf;
         *buf-- = c;
     }

     return p;
}

char* utoa(uint64_t u, char* buf, int base)
{
    if (base < 2 || base > 36) {
        *buf = '\0';
        return buf;
    }

    const char map[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* p = buf, *dp = buf;

     do {
         *buf++ = map[u % base];
         u /= base;
     } while (u);
     *buf-- = '\0';

     while (dp < buf) {
         char c = *dp;
         *dp++ = *buf;
         *buf-- = c;
     }

     return p;
}