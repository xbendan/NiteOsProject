#include <siberix/dvc/device.h>
#include <siberix/fs/partition.h>
#include <stdcxx/linked-list.h>

namespace Kern {
    class DiskDevice : public Device
    {
        virtual int64_t read(uint64_t offset, uint64_t size, void* buffer)  = 0;
        virtual int64_t write(uint64_t offset, uint64_t size, void* buffer) = 0;

    private:
        Std::LinkedList<Partition*> m_partitions;
        uint32_t                    m_blockSize;
    };
}