#include <siberix/drivers/pci/spec.h>

#include <arch/x86_64/iopt.hpp>

PCIInfo::PCIInfo(u8 bus, u8 slot, u8 func)
    : m_bus(bus),
      m_slot(slot),
      m_func(func),
      m_classCode(readByte(PCIClassCode)),
      m_subClass(readByte(PCISubclass)) {}

PCIInfo::~PCIInfo() {}

void PCIInfo::initIO(PCIConfigRegisters reg) {
    outDWord32(0xcf8, PCI_PACKAGE_ADDRESS(bus, slot, func, reg));
}

u8 PCIInfo::readByte(PCIConfigRegisters reg) {
    initIO();

    return (u8)((outDWord32(0xcfc) >> (reg & 2) * 8) & 0xff);
}

u16 PCIInfo::readWord(PCIConfigRegisters reg) {
    initIO();

    return (u16)((inDWord32(0xcfc) >> (offset & 2) * 8) & 0xff);
}

u32 PCIInfo::readDWord(PCIConfigRegisters reg) {
    initIO();

    return inDWord32(0xcfc);
}

void PCIInfo::writeByte(PCIConfigRegisters reg, u8 data) {
    initIO();
    outDWord32(0xcfc,
               (inDWord32(0xcfc) & (~(0xff << ((reg & 3) * 8)))) |
                   (static_cast<u32>(data) << ((reg & 3) * 8)));
}

void PCIInfo::writeWord(PCIConfigRegisters reg, u16 data) {
    initIO();
    outDWord32(0xcfc,
               (inDWord32(0xcfc) & (~(0xffff << ((reg & 2) * 8)))) |
                   (static_cast<u32>(data) << ((reg & 3) * 8)));
}

void PCIInfo::writeDWord(PCIConfigRegisters reg, u32 data) {
    initIO();
    outDWord32(0xcfc, data);
}