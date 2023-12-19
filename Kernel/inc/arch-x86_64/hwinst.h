#include <siberix/hwinst.h>

namespace Kern::Platform::X64 {
    class InstFloatingPoint : public Hal::InstructionSet
    {
    public:
        void enable() override
        {
            asm volatile("movq %%cr0, %%rax\n\t"
                         "andq $0xfffffffb, %%rax\n\t"
                         "orq $0x2, %%rax\n\t"
                         "movq %%rax, %%cr0\n\t"
                         :
                         :
                         : "rax");
        }
        void disable() override
        {
            asm volatile("movq %%cr0, %%rax\n\t"
                         "andq $0xfffffffb, %%rax\n\t"
                         "movq %%rax, %%cr0\n\t"
                         :
                         :
                         : "rax");
        }
    };

    class InstSimd : public Hal::InstructionSet
    {
    public:
        void enable() override
        {
            asm volatile("movq %%cr0, %%rax\n\t"
                         "andq $0xffffffef, %%rax\n\t"
                         "orq $0x10, %%rax\n\t"
                         "movq %%rax, %%cr0\n\t"
                         :
                         :
                         : "rax");
        }
        void disable() override
        {
            asm volatile("movq %%cr0, %%rax\n\t"
                         "andq $0xffffffef, %%rax\n\t"
                         "movq %%rax, %%cr0\n\t"
                         :
                         :
                         : "rax");
        }
    };

    class InstSse : public Hal::InstructionSet
    {
    public:
        void enable() override;
        void disable() override;
    };

    class InstAvx : public Hal::InstructionSet
    {
    public:
        void enable() override;
        void disable() override;
    };

    class InstAvx512 : public Hal::InstructionSet
    {
    public:
        void enable() override;
        void disable() override;
    };
}