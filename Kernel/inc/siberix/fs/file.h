#include <stdcxx/string.h>
#include <stdcxx/types.h>
#include <stdcxx/uuid.h>

namespace Kern::IO::FileSystem {
    class File
    {
    public:
        File() = default;
        File(Std::String<Utf16> fullpath);
        ~File();

    private:
        Std::String<Utf16> m_path;
        Std::String<Utf16> m_name;
        Std::UUID          m_uuid;
        UInt64             m_size;
        UInt64             m_bytesTaken;
    };
}