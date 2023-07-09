#include <common/typedefs.h>

enum VolumeType { VolumeTypeLocalDisk, VolumeTypeRemoteDisk };

enum FileType {
    FileTypeFile,
    FileTypeDirectory,
    FileTypeShortcut,
    FileTypeDisk
};

struct Volume {
    char* name;
    VolumeType volumeType;
    u64 capacity;
    u64 spaceUsed;
};

struct Partition {};

struct File {
    char* name;
    u8 uuid[16];
    FileType type;
    u64 size;
    u64 bytesTaken;
    u64 flags;
};

class DiskService {
public:
    void mount(Volume& volume);
    File listFiles(const char* path);
    bool createFile(const char* path);
    bool deleteFile(const char* path);
    u8* readBytes(const char* path);
    void writeBytes(const char* path, u8* content);
};