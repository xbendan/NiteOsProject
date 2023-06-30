#include <Init/BootInfo.h>
#include <Drivers/video.h>
#include <Drivers/Generic/ACPI.h>
#include <Drivers/Generic/APIC.h>
#include <Proc/Process.h>
#include <System.h>

#include <Arch/x86_64/Boot.h>
#include <Arch/x86_64/Interrupts.h>
#include <Arch/x86_64/CPU.h>
#include <Arch/x86_64/GDT.h>
#include <Arch/x86_64/IDT.h>
#include <Arch/x86_64/SystemManagementBIOS.h>
#include <Arch/x86_64/SMP.h>

extern "C" void asmw_enable_sse();
extern "C" void asmw_enable_avx();

BootInfo bootInfo;

namespace Boot
{
    void multiboot2(BootInfo *info, multiboot2_info_header_t *mbInfo)
    {
        
    }

    void stivale2(BootInfo *info, stivale2_struct_t *stInfo)
    {
        struct stivale2_tag* tag = (struct stivale2_tag*)(stInfo->tags);
        BootInfoMemory *memInfo = &bootInfo.m_Memory;
        while (tag)
        {
            switch (tag->identifier)
            {
            case STIVALE2_STRUCT_TAG_MEMMAP_ID:
            {
                struct stivale2_struct_tag_memmap *st2_mem_tag = (struct stivale2_struct_tag_memmap*)(tag);
                for (uint64_t idx = 0; idx < st2_mem_tag->entries; idx++)
                {
                    struct stivale2_mmap_entry *fromEntry = &st2_mem_tag->memmap[idx];
                    Memory::MemoryMapEntry *newEntry = &memInfo->m_MemoryMapEntries[memInfo->m_MemoryMapSize];

                    if (fromEntry->base > UINTPTR_MAX ||
                        fromEntry->base + fromEntry->length > UINTPTR_MAX)
                    {
                        continue;
                    }

                    memInfo->m_TotalSize += fromEntry->length;
                    newEntry->m_AddrStart = fromEntry->base;
                    newEntry->m_AddrEnd = fromEntry->base + fromEntry->length;
                    switch (fromEntry->type)
                    {
                    case STIVALE2_MMAP_USABLE:
                        memInfo->m_Usable += fromEntry->length;
                        newEntry->m_Type = Memory::MemoryMapEntryTypeAvailable;
                        break; 
                    case STIVALE2_MMAP_KERNEL_AND_MODULES:
                        newEntry->m_Type = Memory::MemoryMapEntryTypeKernel;
                        break;
                    case STIVALE2_MMAP_ACPI_RECLAIMABLE:
                        newEntry->m_Type = Memory::MemoryMapEntryTypeAcpiReclaimable;
                        break;
                    case STIVALE2_MMAP_ACPI_NVS:
                        newEntry->m_Type = Memory::MemoryMapEntryTypeNvs;
                        break;
                    case STIVALE2_MMAP_BAD_MEMORY:
                        newEntry->m_Type = Memory::MemoryMapEntryTypeBadRam;
                        break;
                    default:
                        newEntry->m_Type = Memory::MemoryMapEntryTypeReserved;
                        break;
                    }
                    memInfo->m_MemoryMapSize++;
                }
                break;
            }
            case STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID:
            {
                struct stivale2_struct_tag_framebuffer *framebuffer_tag = (struct stivale2_struct_tag_framebuffer *) tag;
                BootInfoGraphics *graphic = &bootInfo.m_Graphics;

                graphic->m_Width = framebuffer_tag->framebuffer_width;
                graphic->m_Height = framebuffer_tag->framebuffer_height;

                graphic->m_BufferAddress = framebuffer_tag->framebuffer_addr;
                graphic->m_Pitch = framebuffer_tag->framebuffer_pitch;
                graphic->m_BytesPerPixel = framebuffer_tag->framebuffer_bpp;
                break;
            }
            case STIVALE2_STRUCT_TAG_MODULES_ID:
            {
                break;
            }
            case STIVALE2_STRUCT_TAG_RSDP_ID:
            {
                break;
            }
            }
            tag = (struct stivale2_tag*)(tag->next);
        }

        bootInfo.m_Checksum = 0xDEADC0DE;
    }

    void Start()
    {
        if (bootInfo.m_Checksum != 0xDEADC0DE) {
            return;
        }

        DisableInterrupts();

        asmw_enable_sse();

        GDT::Initialize();
        IDT::Initialize();

        Video::Initialize();
        Memory::Initialize();

        CPUIDInfo cpuId = CPUID();
        // Check hardware features
        {
            System::Out("Checking hardware features...");

            if (!(cpuId.edx & CPUID_EDX_MSR)) {
                System::Panic("This processor doesn't support MSR (Model Specific Registers).");
            }

            if (!(cpuId.ecx & CPUID_ECX_SSE3)) {
                System::Panic("This processor doesn't support SSE3!");
            }

            if (!(cpuId.edx & CPUID_EDX_APIC)) {
                System::Panic("This processor doesn't support APIC (Advanced Programmable Interrupt Controller)!");
            }

            System::Out("OK!");
        }

        ACPI::Initialize();
        // APIC
        APIC::Initialize();

        EnableInterrupts();

        SMBIOS::Initialize();
        SMP::Initialize();

        KernelInit();

        for (;;) asm("hlt");
    }
} // namespace Boot

extern "C" [[noreturn]] void kload_stivale2(void *ptr)
{
    if(ptr == NULL)
        __asm__("mov $0x32, %al");

    Boot::stivale2(&bootInfo, (stivale2_struct_t*)(ptr));
    Boot::Start();

hang:
    __asm__("hlt");
    goto hang;
}