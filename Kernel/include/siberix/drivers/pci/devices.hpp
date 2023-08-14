#include <siberix/device/device.h>
#include <siberix/drivers/acpi/spec.h>
#include <siberix/drivers/pci/spec.h>

class PciDevice : public Device {
public:
    PciDevice(u8 bus, u8 slot, u8 func);
    PciDevice(u8 bus, u8 slot, u8 func, u8 classCode, u8 subClass);
    PciDevice(PCIInfo& info);
    ~PciDevice();

    void enable() override;
    void disable() override;

private:
    PCIInfo        m_info;
    MSICompability m_compability;
    u8             m_msiptr;
    bool           m_isMsiCapable;
};

class PciControllerDevice : public Device {
public:
    PciControllerDevice();
    ~PciControllerDevice();

    void enable() override;
    void disable() override;

    u8   configReadByte(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg);
    u16  configReadWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg);
    u32  configReadDWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg);
    void configWriteByte(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u8 data);
    void configWriteWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u16 data);
    void configWriteDWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u32 data);

    bool       checkDevice(u8 bus, u8 device, u8 func);
    PciDevice& connectDevice(u8 bus, u8 device, u8 func);
    PciDevice* findDevice(u16 deviceID, u16 vendorID);
    PciDevice* findGenericDevice(u16 classCode, u16 subclass);
    void       enumerateDevice(u16 deviceID, u16 vendorID, void (*consumer)(PciDevice& device));
    void enumerateGenericDevice(u8 classCode, u8 subclass, void (*consumer)(PciDevice& device));

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
        return configReadByte(info.bus(), info.slot(), info.func(), reg);
    }

    inline u16 configReadWord(PCIInfo& info, PCIConfigRegisters reg) {
        return configReadWord(info.bus(), info.slot(), info.func(), reg);
    }

    inline u32 configReadDWord(PCIInfo& info, PCIConfigRegisters reg) {
        return configReadByte(info.bus(), info.slot(), info.func(), reg);
    }

    inline void configWriteByte(PCIInfo& info, PCIConfigRegisters reg, u8 data) {
        configWriteByte(info.bus(), info.slot(), info.func(), reg, data);
    }

    inline void configWriteWord(PCIInfo& info, PCIConfigRegisters reg, u16 data) {
        configWriteWord(info.bus(), info.slot(), info.func(), reg, data);
    }

    inline void configWriteDWord(PCIInfo& info, PCIConfigRegisters reg, u32 data) {
        configWriteDWord(info.bus(), info.slot(), info.func(), reg, data);
    }

private:
    LinkedList<PciDevice&>   m_deviceList;
    LinkedList<McfgAddress*> m_enhancedAddressList;
    PciMcfg*                 m_mcfgTable;
    PCIConfigAccessMode      m_accessMode;
};