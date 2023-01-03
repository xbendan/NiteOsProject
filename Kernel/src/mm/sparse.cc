#include <mm/mmzone.h>

namespace Memory::Model
{
    mem_section_t sections[SPARSEMEM_LIMIT];

    void MemblockPresent(uint64_t addrStart, uint64_t addrEnd) {
        while (addrStart < addrEnd) {
            sections[]

            addrStart += SIZE_1G;
        }
    }
} // namespace Memory

