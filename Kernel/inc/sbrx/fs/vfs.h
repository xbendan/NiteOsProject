#include <mixins/types.h>

namespace sbrx::fs {
    class VirtualFileSystem
    {};

    class FileSystem
    {};

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
    };

    class FsDirectory : public FsNode
    {};

    class FsFile
    {
    public:
        FsFile() = default;
        FsFile(Std::String<Utf16> fullpath);
        ~FsFile();

        inline Std::String<Utf16> getPath() { return m_path; }
        inline Std::String<Utf16> getName() { return m_name; }
        inline Std::UUID          getUUID() { return m_uuid; }
        inline uint64_t           getSize() { return m_size; }
        inline uint64_t           getBytesTaken() { return m_bytesTaken; }
        Directory*                getParent();
        bool                      isFile();
        bool                      isDirectory();
    };
}