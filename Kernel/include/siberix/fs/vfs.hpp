#include <common/typedefs.h>

enum class VolumeType { Simple, Mirror, Striped, Spanned };

class Volume {
    const char* m_name;
    VolumeType  m_type;
    u64         m_capacity;
    u64         m_spaceUsed;
};

struct Partition {
    const char* m_name;
    u8          m_diskId;
    u8          m_partitionId;
    u64         m_capacity;
    u64         m_spaceUsed;
};

struct File {
    const char* m_name;
    u8          m_uuid[16];
    bool        m_isDirectory;
    u64         m_size;
    u64         m_bytesTaken;
    u64         m_flags;
};

class VirtualFsDevice {
public:
    void mount(Volume& volume);
    File listFiles(const char* path);
    bool createFile(const char* path);
    bool deleteFile(const char* path);
    u8*  readBytes(const char* path);
    void writeBytes(const char* path, u8* content);

private:
    LinkedList<DiskDevice&> m_disks;
    LinkedList<Partition&>  m_partitions;
};