#pragma once

#include <common/typedefs.h>
#include <siberix/fs/disk.h>
#include <siberix/fs/partition.h>

enum class VolumeType {
    Simple,
    Mirror,
    Striped,
    Spanned
};

class Volume {
    const char* m_name;
    VolumeType  m_type;
    u64         m_capacity;
    u64         m_spaceUsed;
};

class File {
public:
    File() = default;
    File(const char*);
    ~File();

    virtual u64  read(u64, u64, u8*);
    virtual u64  write(u64, u64, u8*);
    virtual void close();

    inline const char* getName() { return m_name; }
    inline u8*         getUuid() { return m_uuid; }
    inline bool        isDirectory() { return m_isDirectory; }
    inline u64         getSize() { return m_size; }
    inline u64         getBytesTaken() { return m_bytesTaken; }

private:
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