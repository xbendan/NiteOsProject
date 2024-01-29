#include <arch-x86_64/hwinterrupts.h>
#include <arch-x86_64/hwmem.h>
#include <arch-x86_64/hwtypes.h>
#include <drivers/acpi/device.h>
#include <drivers/apic/device.h>
#include <drivers/comm/device.h>
#include <drivers/pci/enumerator.h>
#include <siberix/init/init-comp.h>
#include <siberix/main.h>
#include <siberix/svc/svc-host.h>
#include <xtra-logging/logger.h>

extern "C"
{
    void _lgdt(uint64_t);
    void _lidt(uint64_t);
}

namespace Kern::Platform::X64 {
    extern IdtEntry idtEntry[256];
}

namespace Kern::Main {
    extern Svc::MemSvcHost*       svcMem;
    extern Svc::TaskSvcHost*      svcTask;
    extern Logger*                logger;
    extern DeviceConnectivity*    connectivity;
    extern Io::VirtualFileSystem* fileSystem;
}

namespace Kern::Init {
    using namespace Kern::Platform::X64;
    using namespace Kern::Hal;

    Mem::AddressSpace* setupVirtMemory()
    {
        static X64KernelPages kernelPages;
        return &kernelPages;
    }

    void setupArch()
    {
        static CPUImplX64 cpu0(0, 0);
        cpu0.disableInterrupts();

        /* load global descriptor table */
        cpu0.m_gdt    = GdtPackage(cpu0.m_tss);
        cpu0.m_gdtPtr = {
            .limit = sizeof(GdtPackage) - 1,
            .base  = (uint64_t)&cpu0.m_gdt,
        };
        _lgdt((uint64_t)&cpu0.m_gdtPtr);
        /* load interrupt descriptor table */
        for (int i = 0; i < IDT_ENTRY_COUNT; i++)
            idtEntry[i] =
              IdtEntry(i, g_interruptTables[i], 0x08, IDT_FLAGS_INTGATE, 0);
        cpu0.m_idtPtr = {
            .limit = sizeof(IdtEntry) * IDT_ENTRY_COUNT,
            .base  = (uint64_t)&idtEntry,
        };
        _lidt((uint64_t)&cpu0.m_idtPtr);

        static Svc::MemSvcHost _svcMem = Svc::MemSvcHost();
        (Main::svcMem = &_svcMem)->onLoad();
        (Main::svcTask = new Svc::TaskSvcHost())->onLoad();
        (Main::logger = new Logger());
        (Main::connectivity = new DeviceConnectivity( //
           new Hal::Impls::SerialPortDevice(),
           new Hal::Impls::AcpiMgmtDevice(),
           new Hal::Impls::ApicDevice(),
           new Hal::Impls::PeriCompDeviceEnumerator()))
          ->onLoad();
        (Main::fileSystem = new Io::VirtualFileSystem());
    }
}