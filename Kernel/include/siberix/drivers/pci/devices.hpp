#include <siberix/drivers/acpi/spec.h>
#include <siberix/drivers/pci/spec.h>

#include <siberix/device/device.hpp>

class PCIDevice : public Device {
public:
    PCIDevice(u8 bus, u8 slot, u8 func);
    PCIDevice(u8 bus, u8 slot, u8 func, u8 classCode, u8 subClass);
    PCIDevice(PCIInfo& info);
    ~PCIDevice();

private:
    PCIInfo        m_info;
    MSICompability m_compability;
    u8             m_msiptr;
    bool           m_isMsiCapable;
};

class PCIControllerDevice : public Device {
public:
    PCIControllerDevice();
    ~PCIControllerDevice();

    u8   configReadByte(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg);
    u16  configReadWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg);
    u32  configReadDWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg);
    void configWriteByte(
        u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u8 data);
    void configWriteWord(
        u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u16 data);
    void configWriteDWord(
        u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u32 data);

    bool       checkDevice(u8 bus, u8 device, u8 func);
    PCIDevice& connectDevice(u8 bus, u8 device, u8 func);
    PCIDevice* findDevice(u16 deviceID, u16 vendorID);
    PCIDevice* findGenericDevice(u16 classCode, u16 subclass);
    void       enumerateDevice(u16 deviceID,
                               u16 vendorID,
                               void (*consumer)(PCIDevice& device));
    void       enumerateGenericDevice(u8 classCode,
                                      u8 subclass,
                                      void (*consumer)(PCIDevice& device));

    u16 getVendorID(u8 bus, u8 slot, u8 func) {
        return configReadWord(bus, slot, func, PCIVendorID);
    }

    u16 getDeviceID(u8 bus, u8 slot, u8 func) {
        return configReadWord(bus, slot, func, PCIDeviceID);
    }

    u8 getHeaderType(u8 bus, u8 slot, u8 func) {
        return configReadWord(bus, slot, func, PCIHeaderType);
    }

    inline u8 configReadByte(PCIInfo& info, PCIConfigRegisters reg) {
        return configReadByte(info.bus, info.slot, info.func, reg);
    }

    inline u16 configReadByte(PCIInfo& info, PCIConfigRegisters reg) {
        return configReadByte(info.bus, info.slot, info.func, reg);
    }

    inline u32 configReadByte(PCIInfo& info, PCIConfigRegisters reg) {
        return configReadByte(info.bus, info.slot, info.func, reg);
    }

    inline void configWriteByte(PCIInfo&           info,
                                PCIConfigRegisters reg,
                                u8                 data) {
        configWriteByte(info.bus, info.slot, info.func, reg, data);
    }

    inline void configWriteWord(PCIInfo&           info,
                                PCIConfigRegisters reg,
                                u16                data) {
        configWriteWord(info.bus, info.slot, info.func, reg, data);
    }

    inline void configWriteDWord(PCIInfo&           info,
                                 PCIConfigRegisters reg,
                                 u32                data) {
        configWriteDWord(info.bus, info.slot, info.func, reg, data);
    }

private:
    LinkedList<PCIDevice&>   m_deviceList;
    LinkedList<McfgAddress*> m_enhancedAddressList;
    PciMcfg*                 m_mcfgTable;
    PCIConfigAccessMode      m_accessMode;
}