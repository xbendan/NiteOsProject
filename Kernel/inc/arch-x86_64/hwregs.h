#include <stdcxx/types.h>

namespace Kern::Platform::X64 {
    struct Registers
    {
        UInt64 r15, r14, r13, r12, r11, r10, r9, r8;
        UInt64 rbp, rdi, rsi, rdx, rcx, rbx, rax;
        UInt64 intno, err;
        UInt64 rip, cs, rflags, rsp, ss;
    } __attribute__((packed));
}
