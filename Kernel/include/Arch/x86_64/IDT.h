#include <stdint.h>

extern "C" void asmw_flush_idt(uint64_t);

namespace IDT
{
    void Initialize();
} // namespace IDT
