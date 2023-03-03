#include <utils/list.h>
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

    typedef enum FileType
    {
        FileTypeFile,
        FileTypeDirectory,
        FileTypeShortcut,
        FileTypeDisk
    } filetype_t;

    struct Volume
    {
        char *m_Name;
        FileSystemType m_FsType;
        VolumeType m_VolType;
        uint64_t m_Capacity;
        uint64_t m_SpaceUsed;
    };

    typedef struct FsPartition 
    {

    } parition_t;

    struct FsNode {};

    typedef struct File
    {
        char *name;
        uint8_t uuid[16];
        filetype_t type;
        uint64_t size;
        uint64_t bytesTaken;
        uint64_t flags;
    } file_t;

    void Mount(Volume *volume);

    File *ListFiles(const char *dir);
    bool Create(const char *path);
    bool Delete(const char *path);
    uint8_t* ReadBytes(const char *path);
    void WriteBytes(const char *path, uint8_t *content);

} // namespace Fs
