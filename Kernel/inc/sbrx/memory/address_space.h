#include <mixins/meta/field.h>
#include <mixins/meta/res.h>
#include <mixins/types.h>
#include <mixins/utils/array.h>
#include <sbrx/memory/page.h>

namespace sbrx::mem {
    using namespace mixins::utils;
    using namespace mixins;

    class AddressSpace
    {
    public:
        AddressSpace()
          : m_stats()
        {
        }
        virtual ~AddressSpace() {}

        virtual Res<PageRec*, PageErr> alloc4KPages(uint64_t amount,
                                                    bool     rw  = true,
                                                    bool     pwt = false,
                                                    bool     pcd = false) = 0;
        virtual Res<void, PageErr>     free4KPages(uint64_t address,
                                                   uint64_t amount)   = 0;
        virtual Res<void, PageErr>     map4KPages(uint64_t phys,
                                                  uint64_t virt,
                                                  uint64_t amount)    = 0;
        virtual Res<bool, PageErr>     isPresent(uint64_t address)    = 0;

        ReadOnly<Array<uint64_t, 5>> stats;

    private:
        Array<uint64_t, 5> m_stats;
    };
}