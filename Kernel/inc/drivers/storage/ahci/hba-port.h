#include <drivers/storage/ahci/spec.h>
#include <siberix/dvc/disk-device.h>

namespace AHCI {
    class HBAPortDevice : public Kern::DiskDevice
    {
    public:
        HBAPortDevice(int n, HbaPort* port, HbaMem* mem);
        ~HBAPortDevice();

        virtual int64_t read(uint64_t offset,
                             uint64_t size,
                             void*    buffer) override;
        virtual int64_t write(uint64_t offset,
                              uint64_t size,
                              void*    buffer) override;

        void start();
        void stop();

    private:
        HbaPort*          m_portRegs;
        HbaCommandHeader* m_commandList;
        HbaCommandTable*  m_commandTable;
        HbaFis*           m_fis;

        uint64_t m_physBuffer[8];
    };
}