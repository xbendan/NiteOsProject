#include <Arch/x86_64/cpu.h>
#include <Proc/Scheduler.h>
#include <Proc/Process.h>

namespace Task
{
    void Scheduler::SwitchThread(Thread *newThread)
    {
        CPU *cpu = GetCPULocal();

        cpu->currentThread = newThread;
        cpu->tss.rsp[0] = reinterpret_cast<uint64_t>(newThread->m_KernelStack);
        
        WriteMsr(0xC0000100 /* Fs Base */, newThread->m_FsBase);
        
        asm volatile(
            R"(mov %0, %%rsp;
            mov %1, %%rax;
            pop %%r15;
            pop %%r14;
            pop %%r13;
            pop %%r12;
            pop %%r11;
            pop %%r10;
            pop %%r9;
            pop %%r8;
            pop %%rbp;
            pop %%rdi;
            pop %%rsi;
            pop %%rdx;
            pop %%rcx;
            pop %%rbx;
            
            mov %%rax, %%cr3
            pop %%rax
            addq $8, %%rsp
            iretq)" ::"r"(&newThread->m_Registers),
            "r"(newThread->m_Parent->m_Pagemap->pml4Phys)
        );
    }
} // namespace Task
