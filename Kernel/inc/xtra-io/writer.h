#include <stdcxx/array.h>
#include <stdcxx/string.h>

namespace Io {
    class Writer
    {
    public:
        virtual void write(Bytes) = 0;
    };

    class TextWriter : public Writer
    {
    public:
        virtual void write(Std::String<Utf8> str) = 0;
    };
}