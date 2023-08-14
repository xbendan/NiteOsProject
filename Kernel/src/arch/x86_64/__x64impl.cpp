#include <arch/x86_64/apic.h>
#include <arch/x86_64/arch.h>
#include <arch/x86_64/paging.h>
#include <arch/x86_64/serial.h>
#include <arch/x86_64/smbios.h>
#include <arch/x86_64/types.h>
#include <siberix/drivers/acpi/acpi_device.h>
#include <siberix/mm/page.h>

#include <siberix/drivers/pci/devices.hpp>

static X64Executive x64rt;
extern "C" void     _lgdt(u64);
extern "C" void     _lidt(u64);

TaskStateSegment              tss = { .rsp = {}, .ist = {}, .iopbOffset = 0 };
GdtPackage                    gdtPackage;
GdtPtr                        gdtPtr;
IdtPtr                        idtPtr;
Paging::X64KernelAddressSpace addressSpace;

bool X64Executive::setupArch() {
    /* load global descriptor table */
    gdtPackage = GdtPackage(tss);
    gdtPtr     = { .limit = sizeof(GdtPackage) - 1, .base = (u64)&gdtPackage };
    _lgdt((u64)&gdtPtr);
    /* load interrupt descriptor table */
    for (int i = 0; i < IDT_ENTRY_COUNT; i++)
        idtEntries[i] = IdtEntry(i, isrTables[i], 0x08, IDT_FLAGS_INTGATE, 0);
    idtPtr = { .limit = sizeof(IdtEntry) * IDT_ENTRY_COUNT, .base = (u64)&idtEntryList };
    _lidt((u64)&idtPtr);

    m_kernelSpace  = &(addressSpace = Paging::X64KernelAddressSpace());
    this->m_memory = MemoryManagement();

    this->m_devices = new DeviceConnectivity();
    (new SerialPortDevice())->initialize();    /* Serial Port */
    (new SmbiosDevice())->initialize();        /* System Management BIOS */
    (new AcpiPmDevice())->initialize();        /* ACPI Power Management */
    (new ApicDevice())->initialize();          /* APIC Controller */
    (new PciControllerDevice())->initialize(); /* PIC Controller */
}

void TaskStateSegment::init(GdtPackage *package) {
    package->tss = GdtTssEntry(*this);

    memset(this, 0, sizeof(TaskStateSegment));

    for (int i = 0; i < 3; i++) {
        ist[i] = (u64)exec()->getMemory().alloc4KPages(8);
        memset((void *)ist[i], 0, PAGE_SIZE_4K);
        ist[i] += PAGE_SIZE_4K * 8;
    }

    asm volatile("mov %%rsp, %0" : "=r"(rsp[0]));
    asm volatile("ltr %%ax" ::"a"(0x28));
}