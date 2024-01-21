#pragma once

#include <stdcxx/string.h>
#include <stdcxx/types.h>
#include <stdcxx/uuid.h>

namespace Kern::Io {

    class Directory;

    enum class FileType : uint8_t
    {
        Unknown = 0,
        File,
        Directory,
        Device,
        Pipe,
        Socket,
        SymbolicLink,
        HardLink,
        MountPoint,
    };

    class FsNode
    {
    public:
        virtual int64_t read(uint64_t offset,
                             uint64_t size,
                             uint8_t* buffer)  = 0;
        virtual int64_t write(uint64_t offset,
                              uint64_t size,
                              uint8_t* buffer) = 0;
        virtual void    close()                = 0;

    private:
        Std::String<Utf16> m_name;
        Std::UUID          m_uuid;
    };

    class RootFsNode : FsNode
    {};

    class File
    {
    public:
        File() = default;
        File(Std::String<Utf16> fullpath);
        ~File();

        inline Std::String<Utf16> getPath() { return m_path; }
        inline Std::String<Utf16> getName() { return m_name; }
        inline Std::UUID          getUUID() { return m_uuid; }
        inline uint64_t           getSize() { return m_size; }
        inline uint64_t           getBytesTaken() { return m_bytesTaken; }
        Directory*                getParent();
        bool                      isFile();
        bool                      isDirectory();

    private:
        Std::String<Utf16> m_path;
        Std::String<Utf16> m_name;
        Std::UUID          m_uuid;
        uint64_t           m_flags;
        uint64_t           m_size;
        uint64_t           m_bytesTaken;
    };

    class Directory : File
    {
    public:
        Directory() = default;
        Directory(Std::String<Utf16> fullpath);
        ~Directory();

        Std::Array<FsNode*>* listFiles();
    };
}