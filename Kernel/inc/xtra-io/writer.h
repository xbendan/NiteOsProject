#pragma once

#include <stdcxx/array.h>
#include <stdcxx/string.h>

class Writer
{
public:
    virtual void write(char c) = 0;
};

class TextWriter : public Writer
{
public:
    virtual void write(Std::String<Utf8> str) = 0;
};