#include <stdint.h>
#include <stddef.h>

extern "C" void asmw_flush_gdt(uint64_t);

namespace GDT
{
    void Initialize();
} // namespace GDT
