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
        using Unit = typename E::Unit;

    public:
        String(const char* str)
          : m_data(reinterpret_cast<Unit*>(const_cast<char*>(str)))
          , m_length(strlen(str)){};
        String(Unit const*, UInt64);
        String(String const&);
        String(String&&);

        String& operator=(const char*);
        String& operator=(String const&);
        String& operator=(String&&);

        bool equals(String const&) const;
        bool equalsIgnoreCase(String const&) const;
        bool isStartWith(String const&) const;
        bool isEndWith(String const&) const;
        bool contains(String const&) const;

        bool operator==(String const&) const;
        bool operator!=(String const&) const;

    private:
        Unit*  m_data;
        UInt64 m_length;
    };
}