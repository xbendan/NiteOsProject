#include <arch-x86_64/hwinterrupts.h>
#include <arch-x86_64/hwtypes.h>
#include <siberix/main.h>
#include <siberix/svc/svc-host.h>

extern "C"
{
    void _lgdt(uint64_t);
    void _lidt(uint64_t);
}

namespace Kern::Platform::X64 {
    extern IdtEntry idtEntry[256];
}

namespace Kern::Init {
    using namespace Kern::Platform::X64;

    CPUX64Impl cpu0;

    void setupArch()
    {
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
    }

    Std::Array<Svc::SvcLoader<Svc::ISvcHost>*>* getLoaders()
    {
        static Std::Array<Svc::SvcLoader<Svc::ISvcHost>*> loaders = {
            new Svc::SvcLoader<Svc::MemSvcHost>(),
            new Svc::SvcLoader<Svc::TaskSvcHost>(),
            new Svc::SvcLoader<Svc::IoSvcHost>(),
        };
        return &loaders;
    }
}