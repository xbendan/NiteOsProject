#include <mixins/meta/ref.h>
#include <mixins/meta/res.h>
#include <mixins/types.h>
#include <sbrx/memory/address_space.h>
#include <sbrx/memory/page.h>

namespace sbrx::main {
    Res<uint64_t, mem::PageErr> alloc4KPages(uint64_t amount);
    Res<uint64_t, mem::PageErr> alloc4KPages(uint64_t           amount,
                                             mem::AddressSpace* addressSpace,
                                             mem::PageRec**     pageReference);
    Res<void, mem::PageErr>     free4KPages(uint64_t address, uint64_t amount);
}