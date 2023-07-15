#include <siberix/drivers/ahci/spec.h>
#include <utils/linked_list.h>

#include <siberix/fs/disk.hpp>

class SATADiskDevice : public DiskDevice {
private:
    HbaPortRegisters*   m_portRegs;
    HbaMemoryRegisters* m_memRegs;
    int                 m_status;
    HbaCommandHeader*   m_commandList;
    HbaCommandTable*    m_commandTable[8];
    HbaFrameInfo*       m_frameInfos;

public:
    SATADiskDevice(int port, HbaPortRegisters* portRegs);
    ~SATADiskDevice();

    u64 read(u64 sector, u64 size, u8* buffer) override;
    u64 write(u64 sector, u64 size, u8* buffer) override;
};

class AHCIControllerDevice {
private:
    LinkedList<SATADiskDevice&> m_diskList;
    u64                         m_addrBase;
    u64                         m_addrVirt;
    HbaMemoryRegisters*         m_hbaMemRegs;
    u64                         m_clbPhys;
    u64                         m_fbPhys;
    u64                         m_ctbaPhys;

public:
    AHCIControllerDevice();
    ~AHCIControllerDevice();
}