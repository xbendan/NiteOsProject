#include <Arch/x86_64/idt.h>
#include <Arch/x86_64/cpu.h>

extern uintptr_t isr_tables[];
static struct InterruptDescTableEntry g_IdtEntryList[IDT_ENTRY_COUNT];
struct InterruptDescTablePointer g_IDTPointer = {
    .size = sizeof(struct InterruptDescTableEntry) * IDT_ENTRY_COUNT,
    .base = (uint64_t) &g_IdtEntryList[0]
};

namespace IDT
{
    void SetGate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags, uint8_t ist = 0) {
        g_IdtEntryList[num] = {
            .base_low = (uint16_t)(base & 0xffff),
            .selector = sel,
            .ist = ist,
            .flags = flags,
            .base_med = (uint16_t)((base >> 16) & 0xffff),
            .base_high = (uint32_t)((base >> 32) & 0xffffffff),
        };
    }

    void Initialize()
    {
        for (uint8_t num = 0; num < 48; num++)
            SetGate(num, isr_tables[num], 0x08, IDT_FLAGS_INTGATE, 0);
            // g_IdtEntryList[num] = (struct InterruptDescTableEntry){
            //     .base_low = (isr_tables[num] & 0xffff),
            //     .selector = 0x08,
            //     .ist = 0x0,
            //     .flags = IDT_FLAGS_INTGATE,
            //     .base_med = ((isr_tables[num] >> 16) & 0xffff),
            //     .base_high = ((isr_tables[num] >> 32) & 0xffffffff)
            // };
            

        // g_IdtEntryList[127] = (struct InterruptDescTableEntry){
        //     .base_low = (isr_tables[32] & 0xffff),
        //     .selector = 0x08,
        //     .ist = 0x0,
        //     .flags = IDT_FLAGS_INTGATE,
        //     .base_med = ((isr_tables[32] >> 16) & 0xffff),
        //     .base_high = ((isr_tables[32] >> 32) & 0xffffffff)
        // };
        // g_IdtEntryList[128] = (struct InterruptDescTableEntry){
        //     .base_low = (isr_tables[33] & 0xffff),
        //     .selector = 0x08,
        //     .ist = 0x0,
        //     .flags = IDT_FLAGS_INTGATE | IDT_FLAGS_USER,
        //     .base_med = ((isr_tables[33] >> 16) & 0xffff),
        //     .base_high = ((isr_tables[33] >> 32) & 0xffffffff)
        // };

        asmw_flush_idt((uint64_t) &g_IDTPointer);
    }
} // namespace IDT