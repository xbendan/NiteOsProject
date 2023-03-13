#include <dev/device.h>
#include <Utils/LinkedList.h>
#include <macros>

#define FILE_ATTR_READONLY  1
#define FILE_ATTR_HIDDEN    (1 << 1)
#define FILE_ATTR_SYSTEM    (1 << 2)
#define FILE_ATTR_DIRECTORY (1 << 3)

namespace Fs
{
    enum FileSystemType
    {
        FileSystemExFAT,
        FileSystemExt4,
        FileSystemFAT32,
        FileSystemNTFS,
        FileSystemRamFS,
        FileSystemISO9660,
        FileSystemUnknown
    };

    enum VolumeType
    {
        VolumeTypeLocalDisk,
        VolumeTypeRemoteDisk
    };

    enum FileType
    {
        FileTypeFile,
        FileTypeDirectory,
        FileTypeShortcut,
        FileTypeDisk
    };

    struct Volume
    {
        char *m_Name;
        FileSystemType m_FsType;
        VolumeType m_VolType;
        uint64_t m_Capacity;
        uint64_t m_SpaceUsed;
    };

    struct Partition 
    {

    };
    class DiskDevice : public Device
    {
    private:
        uint32_t m_BlockSize;
    public:
        LinkedList<Partition> m_Partitions;

        virtual size_t Read(size_t sector, size_t size, uint8_t *buffer);
        virtual size_t Write(size_t sector, size_t size, uint8_t *buffer);
    };

    struct File
    {
        char *m_Name;
        uint8_t m_UUID[16];
        FileType m_Type;
        uint64_t m_Size;
        uint64_t m_BytesTaken;
        uint64_t m_Flags;
    };
    
    void Mount(Volume *volume);

    File *ListFiles(const char *dir);
    bool Create(const char *path);
    bool Delete(const char *path);
    uint8_t* ReadBytes(const char *path);
    void WriteBytes(const char *path, uint8_t *content);

} // namespace Fs
