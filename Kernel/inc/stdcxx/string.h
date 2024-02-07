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
        {
            int i = 0;
            while (str[i] != '\0') {
                i++;
            }
            m_length = i;
        }
        String(Unit const*, uint64_t);
        String(String const& other)
          : m_data(other.m_data)
          , m_length(other.m_length)
        {
        }
        String(String&& other) noexcept
          : m_data(other.m_data)
          , m_length(other.m_length)
        {
            other.m_data   = nullptr;
            other.m_length = 0;
        }

        String<E>& operator=(const char* str)
        {
            m_data = reinterpret_cast<Unit*>(const_cast<char*>(str));
            int i  = 0;
            while (str[i] != '\0') {
                i++;
            }
            m_length = i;
            return *this;
        }
        String<E>& operator=(String<E> const& other)
        {
            m_data   = other.m_data;
            m_length = other.m_length;
            return *this;
        }
        String<E>& operator=(String<E>&& other)
        {
            m_data   = other.m_data;
            m_length = other.m_length;

            other.m_data   = nullptr;
            other.m_length = 0;
            return *this;
        }

        bool equals(String<E> const& other) const
        {
            if (m_length != other.m_length || !m_data || !other.m_data) {
                return false;
            } else {
                for (int i = 0; i < m_length; i++) {
                    if (m_data[i] != other.m_data[i]) {
                        return false;
                    }
                }
                return true;
            }
        }

        bool equalsIgnoreCase(String<E> const& other) const
        {
            if (m_length != other.length() || !m_data || !other.m_data) {
                return false;
            }
            // for (int i = 0; i < m_length; i++) {
            //     if (E::toLower(m_data[i]) != Utf8::toLower(other.m_data[i]))
            //     {
            //         return false;
            //     }
            // }
            return true;
        }

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