#pragma once

#include <common/format.h>
#include <common/string.h>

class String
{
public:
    String() = default;
    String(const char* str)
      : m_string(const_cast<char*>(str))
      , m_length(strlen(str))
    {
    }
    ~String() { delete m_string; }

    int length() { return m_length; }

    bool isInteger() { return false; }

    bool isDigit() { return false; }

    bool startWith(const char* str)
    {
        return strncmp(m_string, str, strlen(str)) == 0;
    }

    bool endsWith(const char* str)
    {
        return strncmp(&(m_string[m_length - strlen(str)]), str, strlen(str)) ==
               0;
    }

    String& toUpperCase();

    String& toLowerCase();

    String& replace();

    String& format(va_list args);

    String& operator=(String& string)
    {
        this->m_string = new char[strlen(string.m_string)];
        strcpy(m_string, string.m_string);
        m_length = string.m_length;
    }

    String& operator=(const char* string)
    {
        m_string = const_cast<char*>(string);
        m_length = strlen(string);
    }

    String& operator=(char* string)
    {
        m_string = string;
        m_length = strlen(string);
    }

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
    char* m_string;
    int   m_length;
};