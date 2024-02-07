#pragma once

#include <stdcxx/array.h>
#include <stdcxx/string.h>

class Writer
{
public:
    virtual void write(uint8_t) = 0;
};

class TextWriter : virtual public Writer
{
public:
    virtual void write(Std::String<Utf8>) = 0;
};