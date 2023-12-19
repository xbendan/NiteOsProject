#include <arch-x86_64/apic.h>

namespace Kern::Platform::X64::Apic {
    class ApicTimer
    {
    private:
        Interface& m_interface;
        UInt64     m_busClock;
        UInt64 volatile m_ticks;
    };
}