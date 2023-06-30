#include <Arch/x86_64/GDT.h>
#include <Arch/x86_64/CPU.h>

#include <macros>

TaskStateSegment tss = {
    .ign_0 = 0,
    .rsp = {},
    .ign_1 = 0,
    .ist = {},
    .ign_2 = 0,
    .ign_3 = 0,
    .ign_4 = 0,
    .iopb_offset = 0,
};
GDTPackage gdt = {
    {
		{0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00},
		{0xFFFF, 0x0000, 0x00, 0x9A, (1 << 5) | (1 << 7) | 0x0F, 0x00},
		{0xFFFF, 0x0000, 0x00, 0x92, (1 << 5) | (1 << 7) | 0x0F, 0x00},
		{0xFFFF, 0x0000, 0x00, 0xFA, (1 << 5) | (1 << 7) | 0x0F, 0x00},
		{0xFFFF, 0x0000, 0x00, 0xF2, (1 << 5) | (1 << 7) | 0x0F, 0x00},
	},
    {}
};
GDTPointer g_GDTPointer = {
    .size = sizeof(GDTPackage) - 1,
    .base = (uint64_t) &gdt,
};

namespace GDT
{
    void Initialize()
    {
        gdt.tss = GDTTssEntry(tss);

        asmw_flush_gdt((uint64_t) &g_GDTPointer);
    }
} // namespace GDT