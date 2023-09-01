#include <arch/x86_64/apic.h>
#include <arch/x86_64/arch.h>
#include <arch/x86_64/iopt.h>
#include <arch/x86_64/paging.h>
#include <common/logger.h>
#include <common/string.h>
#include <siberix/drivers/acpi/acpi_device.h>
#include <siberix/proc/sched.h>

#include <arch/x86_64/smpdefines.inc>

Scheduler::Scheduler() {}

Scheduler::Scheduler(Process* kernelProcess)
    : m_nextPID(1),
      m_timeSlice(50),
      m_kernelProcess(kernelProcess),
      m_processFactory(new ProcessFactory()) {}

Scheduler::~Scheduler() {}

void Scheduler::switchThread(Thread* t) {
    Cpu*       cpu    = getCpuLocal();
    X64Thread* thread = static_cast<X64Thread*>(t);

    asm volatile("fxrstor64 (%0)" ::"r"((u64)thread->fxState) : "memory");
    writeMSR(0xc0000100 /* Fs Base */, thread->fsBase);
    Process* process = thread->m_parent;

    cpu->currentThread = thread;
    cpu->tss.rsp[0]    = reinterpret_cast<u64>(thread->kernelStack);

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
            iretq)" ::"r"(&thread->registers),
        "r"(reinterpret_cast<Paging::X64AddressSpace*>(process->getAddressSpace())->pml4Phys));
}

bool Scheduler::addProcess(Process* process) {
    if (m_processList[process->getProcessId()] != nullptr) {
        return false;
    }
    m_processList[process->getProcessId()] = process;
    return true;
}

Process* Scheduler::getKernelProcess() { return m_kernelProcess; }

ProcessFactory* Scheduler::getProcessFactory() { return m_processFactory; }

ThreadQueue& Scheduler::getThreadQueue() { return m_queue; }

Process* thisProcess() { return getCpuLocal()->currentThread->m_parent; }

Thread* thisThread() { return getCpuLocal()->currentThread; }