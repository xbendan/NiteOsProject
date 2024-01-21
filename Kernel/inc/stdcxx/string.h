#pragma once

#include <stdcxx/encoding.h>

extern "C"
{
    void* memset(void* src, int c, unsigned long count);
    void* memcpy(void* dest, const void* src, unsigned long count);
    int   memcmp(const void* s1, const void* s2, unsigned long n);

    void strcpy(char* dest, const char* src);
    void strncpy(char* dest, const char* src, unsigned long n);
    int  strcmp(const char* s1, const char* s2);

    char* strtok_r(char* str, const char* delim, char** saveptr);

    unsigned long strlen(const char* str);
    char*         strcat(char* dest, const char* src);
    char*         strncat(char* dest, const char* src, unsigned long n);

    int strncmp(const char* s1, const char* s2, unsigned long n);

    char* strupr(char* s);
    char* strnupr(char* s, unsigned long n);

    char* strchr(const char* s, int c);
    char* strnchr(const char* s, int c, unsigned long n);
    char* strrchr(const char* s, int c);

    char* strdup(const char* s);
}

namespace Std {
    template <Encoding E>
    class String
    {
        using Encoding = E;
        using Unit     = typename E::Unit;

    public:
        String(const char* str)
          : m_data(reinterpret_cast<Unit*>(const_cast<char*>(str)))
          , m_length(strlen(str)){};
        String(Unit const*, uint64_t);
        String(String const&);
        String(String&&);

        String& operator=(const char*);
        String& operator=(String const&);
        String& operator=(String&&);

        bool equals(String const& other) const
        {
            if (m_length != other.m_length) {
                return false;
            } else {
                if (Unit == other::Unit) {
                    for (uint64_t i = 0; i < m_length; i++) {
                        if (m_data[i] != other.m_data[i]) {
                            return false;
                        }
                    }
                    return true;
                } else {
                    // TODO: Implement
                    return false;
                }
            }
        }

        bool equalsIgnoreCase(String const&) const;
        bool isStartWith(String const&) const;
        bool isEndWith(String const&) const;
        bool contains(String const&) const;

        Unit const& operator[](uint64_t i) const { return m_data[i]; }

        bool operator==(String const& other) const { return equals(other); }
        bool operator!=(String const&) const;

    private:
        Unit*    m_data;
        uint64_t m_length;
    };
}