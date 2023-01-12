#include <Arch/x86_64/pci.h>

namespace PCI
{
    void MsiCapSetData(MSICapability *msiCap, uint32_t dat)
    {
        if (msiCap->msiControl & PCI_CAP_MSI_CONTROL_64)
        {
            msiCap->data64 = dat;
        }
        else
        {
            msiCap->data = dat;
        }
    }

    uint32_t MsiCapGetData(MSICapability *msiCap)
    {
        if (msiCap->msiControl & PCI_CAP_MSI_CONTROL_64)
        {
            return msiCap->data64;
        }
        else
        {
            return msiCap->data;
        }
    }

    void MsiCapSetAddress(MSICapability *msiCap, int cpu)
    {
        msiCap->addressLow = PCI_CAP_MSI_ADDRESS_BASE | (((uint32_t) cpu) << 12);
        if (msiCap->msiControl & PCI_CAP_MSI_CONTROL_64)
        {
            msiCap->addressHigh = 0;
        }
    }

    void SetDevice(PCIDevice *info, uint8_t bus, uint8_t slot, uint8_t func);
    void SetDeviceFromInfo(const PCIInfo *info);

    void ReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    uint32_t ConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    uint16_t ConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    void ConfigWriteWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t data);

    uint8_t ConfigReadByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    void ConfigWriteByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint8_t data);

    bool CheckDevice(uint8_t bus, uint8_t device, uint8_t func);
    bool FindDevice(uint16_t deviceID, uint16_t vendorID);
    bool FindGenericDevice(uint16_t classCode, uint16_t subclass);

    void EnumerateDevice(uint16_t deviceID, uint16_t vendorID, void(*func)(const PCIInfo *info));
    void EnumerateGenericDevice(uint8_t classCode, uint8_t subclass, void(*func)(const PCIInfo *info));

    void Initialize()
    {

    }
} // namespace PCI
