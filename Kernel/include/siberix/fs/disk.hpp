#include <siberix/device/device.h>

#include <siberix/fs/vfs.hpp>

class DiskDevice : public Device {
public:
    LinkedList<Partition&> partitions;

    virtual u64 read(u64 sector, u64 size, u8* buffer);
    virtual u64 write(u64 sector, u64 size, u8* buffer);

private:
    u32 blockSize;
};