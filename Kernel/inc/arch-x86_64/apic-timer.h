#include <arch-x86_64/apic.h>

namespace Kern::Platform::X64::Apic {
    class ApicTimer
    {
    private:
        Interface& m_interface;
        uint64_t     m_busClock;
        uint64_t volatile m_ticks;
    };
}