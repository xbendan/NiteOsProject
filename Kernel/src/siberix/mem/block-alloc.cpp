#include <siberix/init/boot-info.h>
#include <siberix/main.h>
#include <siberix/mem/mem-alloc.h>

namespace Kern::Mem {

    BlockAlloc::BlockAlloc()
    {
        Init::BootConfigTable::Mem&         mem = Main::getBootConfig()->_mem;
        int                                 i   = 0;
        Init::BootConfigTable::Mem::Memmap& memmap = mem._ranges[0];
        while (!memmap._value._max) {
            if (memmap._type != BOOT_MEMMAP_USABLE ||
                memmap._value._max - memmap._value._min < PAGE_SIZE_4K) {
                continue;
            }
            m_pageRange[i] = AddressRange( //
              alignUp(memmap._value._min, PAGE_SIZE_4K),
              alignDown(memmap._value._max, PAGE_SIZE_4K));
        }
    }
}