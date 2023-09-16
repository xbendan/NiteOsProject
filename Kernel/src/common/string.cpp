#include <common/string.h>
#include <common/typedefs.h>

extern "C"
{
    void* memset(void* src, int c, unsigned long count)
    {
        u8* xs = (u8*)src;

        while (count--)
            *xs++ = c;

        return src;
    }

    void* memcpy(void* dest, const void* src, unsigned long count)
    {
        const char* sp = (char*)src;
        char*       dp = (char*)dest;
        for (unsigned long i = count; i >= sizeof(u64); i = count) {
            *((u64*)dp)  = *((u64*)sp);
            sp           = sp + sizeof(u64);
            dp           = dp + sizeof(u64);
            count       -= sizeof(u64);
        }

        for (unsigned long i = count; i >= 4; i = count) {
            *((u32*)dp)  = *((u32*)sp);
            sp           = sp + 4;
            dp           = dp + 4;
            count       -= 4;
        }

        for (unsigned long i = count; i > 0; i = count) {
            *(dp++) = *(sp++);
            count--;
        }

        return dest;
    }

    int memcmp(const void* s1, const void* s2, unsigned long n)
    {
        const u8* a = (u8*)s1;
        const u8* b = (u8*)s2;

        for (unsigned long i = 0; i < n; i++) {
            if (a[i] < b[i]) {
                return -1;
            } else if (a[i] > b[i]) {
                return 1;
            }
        }

        return 0;
    }

    void strcpy(char* dest, const char* src)
    {
        while (*src) {
            *(dest++) = *(src++);
        }
        *dest = 0;
    }

    void strncpy(char* dest, const char* src, unsigned long n)
    {
        while (n-- && *src) {
            *(dest++) = *(src++);
        }
        *dest = 0;
    }

    unsigned long strlen(const char* str)
    {
        unsigned long i = 0;
        while (str[i])
            i++;
        return i;
    }

    int strcmp(const char* s1, const char* s2)
    {
        while (*s1 == *s2) {
            if (!*(s1++)) {
                return 0; // Null terminator
            }

            s2++;
        }
        return (*s1) - *(s2);
    }

    int strncmp(const char* s1, const char* s2, unsigned long n)
    {
        for (unsigned long i = 0; i < n; i++) {
            if (s1[i] != s2[i]) {
                return s1[i] < s2[i] ? -1 : 1;
            } else if (s1[i] == '\0') {
                return 0;
            }
        }

        return 0;
    }

    // strchr - Get pointer to first occurance of c in string s
    char* strchr(const char* s, int c)
    {
        while (*s != (char)c)
            if (!*s++)
                return nullptr;
        return (char*)s;
    }

    // strnchr - Get pointer to first occurance of c in string s, searching at
    // most n characters
    char* strnchr(const char* s, int c, unsigned long n)
    {
        while (n-- && *s != (char)c)
            if (!*s++)
                return nullptr;

        if (n <= 0) {
            return nullptr;
        }

        return const_cast<char*>(s);
    }

    // strrchr - Get pointer to last occurance of c in string s
    char* strrchr(const char* s, int c)
    {
        const char* occ = nullptr;
        while (*s)
            if (*s++ == c)
                occ = s;
        return const_cast<char*>(occ);
    }

    // strspn - Get initial length of s1 including only the characters of s2
    unsigned long strspn(const char* s1, const char* s2)
    {
        unsigned long ret = 0;
        while (*s1 && strchr(s2, *s1++))
            ret++;
        return ret;
    }

    // strspn - Get initial length of s1 excluding the characters of s2
    unsigned long strcspn(const char* s1, const char* s2)
    {
        unsigned long ret = 0;
        while (*s1)
            if (strchr(s2, *s1))
                return ret;
            else
                s1++, ret++;
        return ret;
    }

    // strtok - reentrant strtok
    char* strtok_r(char* str, const char* delim, char** saveptr)
    {
        char*& p = *saveptr;
        if (str)
            p = str;
        else if (!p)
            return 0;
        str = p + strspn(p, delim);
        p   = str + strcspn(str, delim);
        if (p == str)
            return p = 0;
        p = *p ? * p = 0, p + 1 : 0;
        return str;
    }

    char* strcat(char* dest, const char* src)
    {
        strcpy(dest + strlen(dest), src);
        return dest;
    }

    char* strncat(char* dest, const char* src, unsigned long n)
    {
        strncpy(dest + strlen(dest), src, n);
        return dest;
    }

    char toupper(char c)
    {
        if (c >= 'a' && c <= 'z')
            return c - 'a' + 'A';
        else
            return c;
    }

    char* strupr(char* s)
    {
        for (unsigned long i = 0; s[i] != '\0'; i++) {
            s[i] = toupper(s[i]);
        }
        return s;
    }

    char* strnupr(char* s, unsigned long n)
    {
        for (unsigned long i = 0; s[i] && i < n; i++) {
            if (!(i < n))
                break;
            s[i] = toupper(s[i]);
        }

        return s;
    }

    /*
    char* strdup(const char* s) {
        char* buf = new char[strlen(s) + 1];

        strcpy(buf, s);

        return buf;
    }
    */
}