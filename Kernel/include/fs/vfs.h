#include <macros>

namespace Fs
{
    enum FileSystemType
    {
        FileSystemExFAT,
        FileSystemFAT32,
        FileSystemNTFS,
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
        enum FileSystemType m_FsType;
        enum VolumeType m_VolType;
        uint64_t m_Capacity;
        uint64_t m_SpaceUsed;
    };

    typedef struct FsPartition 
    {

    } parition_t;

    struct FsNode {};

    typedef struct File
    {
        char *m_Name;
        uint8_t m_UUID[16];
        filetype_t m_Type;
        uint64_t m_Size;
        uint64_t m_BytesTaken;
    } file_t;

    void ount();
} // namespace Fs
