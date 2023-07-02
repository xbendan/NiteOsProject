#include <common/arch.h>
#include <siberix/mm/page.hpp>

class X64RuntimeSupport : public RuntimeArchSupport {
public:
    X64RuntimeSupport();
    ~X64RuntimeSupport();

    void setup() override;

private:
    Memory::PageAlloc pmmAllocator;
};