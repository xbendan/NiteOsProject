#include <stdcxx/types.h>

namespace Kern::Io {
    class DiskDevice;

    class Partition
    {
    public:
        Partition(DiskDevice* device, uint64_t lbaStart, uint64_t lbaEnd);
        ~Partition();

        uint64_t getStart() const { return m_lbaStart; }
        uint64_t getEnd() const { return m_lbaEnd; }

        uint64_t read(uint64_t offset, uint64_t size, void* buffer);
        uint64_t write(uint64_t offset, uint64_t size, void* buffer);

    private:
        uint64_t m_lbaStart;
        uint64_t m_lbaEnd;
    };
}