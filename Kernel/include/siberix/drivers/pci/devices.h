#include <siberix/device/device.h>
#include <siberix/drivers/acpi/spec.h>
#include <siberix/drivers/pci/spec.h>

class PCIDevice : public Device {
public:
    PCIDevice(u8 bus, u8 slot, u8 func);
    PCIDevice(u8 bus, u8 slot, u8 func, u8 classCode, u8 subClass);
    PCIDevice(PCIInfo& info);
    ~PCIDevice();

    void enable() override;
    void disable() override;

    inline u8 bus() { return m_info.bus(); }
    inline u8 slot() { return m_info.slot(); }
    inline u8 func() { return m_info.func(); }

    inline u16 getDeviceID() { return m_info.getDeviceID(); }
    inline u16 getVendorID() { return m_info.getVendorID(); }

    inline u8  readByte(PCIConfigRegisters reg) { return m_info.readByte(reg); }
    inline u16 readWord(PCIConfigRegisters reg) { return m_info.readWord(reg); }
    inline u32 readDWord(PCIConfigRegisters reg) { return m_info.readDWord(reg); }

    inline void writeByte(PCIConfigRegisters reg, u8 data) { m_info.writeByte(reg, data); }
    inline void writeWord(PCIConfigRegisters reg, u16 data) { m_info.writeWord(reg, data); }
    inline void writeDWord(PCIConfigRegisters reg, u32 data) { m_info.writeDWord(reg, data); }

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

    void enable() override;
    void disable() override;

    u8   configReadByte(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg);
    u16  configReadWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg);
    u32  configReadDWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg);
    void configWriteByte(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u8 data);
    void configWriteWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u16 data);
    void configWriteDWord(u8 bus, u8 slot, u8 func, PCIConfigRegisters reg, u32 data);

    bool       checkDevice(u8 bus, u8 device, u8 func);
    PCIDevice& connectDevice(u8 bus, u8 device, u8 func);
    PCIDevice* findDevice(u16 deviceID, u16 vendorID);
    PCIDevice* findGenericDevice(u16 classCode, u16 subclass);
    void       enumerateDevice(u16 deviceID, u16 vendorID, void (*consumer)(PCIDevice& device));
    void enumerateGenericDevice(u8 classCode, u8 subclass, void (*consumer)(PCIDevice& device));

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
    LinkedList<PCIDevice&>   m_deviceList;
    LinkedList<McfgAddress*> m_enhancedAddressList;
    PciMcfg*                 m_mcfgTable;
    PCIConfigAccessMode      m_accessMode;
};