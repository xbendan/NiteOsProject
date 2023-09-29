#pragma once

#include <common/format.h>
#include <common/string.h>

class String
{
public:
    String() = default;
    String(const char* str)
      : m_string(str)
      , m_length(strlen(str))
    {
    }
    ~String() { delete m_string; }

    int length() { return m_length; }

    String& format(va_list args);

    String& operator+=(String& string)
    {
        int   newLength = m_length + string.m_length + 1;
        char* newString = new char[newLength];

        strcpy(newString, m_string);
        strcpy(&(newString[m_length]), string.m_string);

        delete m_string;

        m_string = newString;
        m_length = newLength;

        newString[m_length] = '\0';

        return *this;
    }

private:
    const char* m_string;
    int         m_length;
};