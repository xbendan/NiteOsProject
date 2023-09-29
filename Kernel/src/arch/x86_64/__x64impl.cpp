#include <arch/x86_64/apic.h>
#include <arch/x86_64/arch.h>
#include <arch/x86_64/clocks.h>
#include <arch/x86_64/interrupts.h>
#include <arch/x86_64/iopt.h>
#include <arch/x86_64/paging.h>
#include <arch/x86_64/serial.h>
#include <arch/x86_64/smbios.h>
#include <arch/x86_64/types.h>
#include <common/format.h>
#include <siberix/display/types/vga.h>
#include <siberix/drivers/acpi/acpi_device.h>
#include <siberix/drivers/pci/devices.h>
#include <siberix/mm/page.h>

#include <arch/x86_64/smpdefines.inc>

extern "C" void _lgdt(u64);
extern "C" void _lidt(u64);

extern void* SMPTrampolineStart;
extern void* SMPTrampolineEnd;

volatile u16* smpMagicValue      = (u16*)SMP_TRAMPOLINE_DATA_START_FLAG;
volatile u16* smpTrampolineCpuID = (u16*)SMP_TRAMPOLINE_CPU_ID;
GdtPtr*       smpGdtPtr          = (GdtPtr*)SMP_TRAMPOLINE_GDT_PTR;
volatile u64* smpRegisterCR3     = (u64*)SMP_TRAMPOLINE_CR3;
volatile u64* smpStack           = (u64*)SMP_TRAMPOLINE_STACK;
volatile u64* smpEntry2          = (u64*)SMP_TRAMPOLINE_ENTRY2;
volatile bool doneInit           = false;

SbrxkrnlX64Impl                sbrxkrnl;
TaskStateSegment               tss = { .rsp = {}, .ist = {}, .iopbOffset = 0 };
Process                        kernelProcess;
Paging::X64KernelAddressSpace* kernelAddressSpace;
ApicDevice*                    _apic;
SerialPortDevice               _serialPort;
SerialPortLoggerReceiver       _serialPortReceiver;

SiberixKernel*
siberix()
{
    return &sbrxkrnl;
}

void
trampolineStart(u16 cpuId)
{
    SbrxkrnlX64Impl* sbrxkrnl = static_cast<SbrxkrnlX64Impl*>(siberix());
    Cpu*             cpu      = getCpuLocal();

    setCpuLocal(cpu);

    cpu->gdt =
      reinterpret_cast<GdtPackage*>(sbrxkrnl->getMemory().alloc4KPages(1));
    memcpy(
      cpu->gdt, (void*)sbrxkrnl->m_gdtPtr.base, sbrxkrnl->m_gdtPtr.limit + 1);
    cpu->gdtPtr = { .limit = sbrxkrnl->m_gdtPtr.limit, .base = (u64)cpu->gdt };
    cpu->idtPtr = { .limit = sbrxkrnl->m_idtPtr.limit,
                    .base  = (u64)sbrxkrnl->m_idtPtr.base };

    asm volatile("lgdt (%%rax)" ::"a"(&cpu->gdtPtr));
    asm volatile("lidt %0" ::"m"(cpu->idtPtr));

    cpu->tss.init(cpu->gdt);
    ApicDevice* apic = static_cast<ApicDevice*>(
      sbrxkrnl->getConnectivity()->findDevice("APIC Controller"));
    if (apic != nullptr) {
        apic->getInterface(cpuId).setup();
        Logger::getLogger("apic").success("CPU [%u] loaded", cpuId);
    }

    asm("sti");
    doneInit = true;

    for (;;)
        asm volatile("pause");
}

bool
SiberixKernel::setupArch()
{
    asm("cli");
    SbrxkrnlX64Impl* k = reinterpret_cast<SbrxkrnlX64Impl*>(this);
    /* load global descriptor table */
    k->m_gdt           = GdtPackage(tss);
    k->m_gdtPtr = { .limit = sizeof(GdtPackage) - 1, .base = (u64)&k->m_gdt };
    _lgdt((u64)&k->m_gdtPtr);
    /* load interrupt descriptor table */
    for (int i = 0; i < IDT_ENTRY_COUNT; i++)
        idtEntryList[i] = IdtEntry(i, intTables[i], 0x08, IDT_FLAGS_INTGATE, 0);
    k->m_idtPtr = { .limit = sizeof(IdtEntry) * IDT_ENTRY_COUNT,
                    .base  = (u64)&idtEntryList };
    _lidt((u64)&k->m_idtPtr);

    kernelProcess = Process("SiberixKernel", nullptr, 0, TaskType::System);
    // Initialize memory management
    static Paging::X64KernelAddressSpace _kernelAddressSpace =
      Paging::X64KernelAddressSpace();
    kernelProcess.setAddressSpace(kernelAddressSpace = &_kernelAddressSpace);

    /* These part are for debug use */
    _serialPort         = SerialPortDevice();
    _serialPortReceiver = SerialPortLoggerReceiver(&_serialPort);

    Logger::initialize();
    Logger::getLoggerReceivers()->add(&_serialPortReceiver);

    // this->m_timers    = LinkedList<TimerDevice*>();
    this->m_memory    = MemoryServiceProvider();
    this->m_devices   = new DeviceConnectivity();
    this->m_scheduler = new Scheduler(&kernelProcess);

    k->m_cpus[0] = new Cpu{
        .apicId        = 0,
        .gdt           = &k->m_gdt,
        .gdtPtr        = k->m_gdtPtr,
        .idtPtr        = k->m_idtPtr,
        .currentThread = getScheduler()->getKernelProcess()->getMainThread(),
        .idleThread    = getScheduler()->getProcessFactory()->createIdleThread()
    };
    k->m_cpus[0]->tss.init(&k->m_gdt);
    setCpuLocal(k->m_cpus[0]);

    asm("sti");

    // (new SerialPortDevice())->initialize();    /* Serial Port */
    new SmbiosDevice();        /* System Management BIOS */
    new AcpiPmDevice();        /* ACPI Power Management */
    new PciControllerDevice(); /* PCI Controller */

    memcpy((void*)(SMP_TRAMPOLINE_ENTRY),
           &SMPTrampolineStart,
           ((u64)&SMPTrampolineEnd) - ((u64)&SMPTrampolineStart));

    _apic = new ApicDevice(); /* APIC Controller */
    getTimeNClock().addTimer(new IntervalTimerDevice(1000), true);
    getConnectivity()
      ->enumerateDevice(DeviceType::Processor)
      .forEach([&](Device& device) -> void {
          u32 processorId =
            static_cast<ProcessorDevice&>(device).getProcessorId();
          if (processorId) {
              ApicLocalInterface& interface = _apic->getInterface(processorId);
              Logger::getLogger("hw").info(
                "CPU [%u] is being initialized, apic id [%u]\n",
                processorId,
                interface.getApicId());

              *smpMagicValue      = 0;
              *smpTrampolineCpuID = processorId;
              *smpEntry2          = (u64)trampolineStart;
              *smpStack           = (u64)(m_memory.alloc4KPages(4)) + 16384;
              *smpGdtPtr          = k->m_gdtPtr;

              asm volatile("mov %%cr3, %%rax" : "=a"(*smpRegisterCR3));

              interface.sendInterrupt(ICR_DSH_DEST, ICR_MESSAGE_TYPE_INIT, 0);
              getDefaultTimer()->sleep(50);
              while (*smpMagicValue != 0xb33f) {
                  interface.sendInterrupt(ICR_DSH_DEST,
                                          ICR_MESSAGE_TYPE_STARTUP,
                                          (SMP_TRAMPOLINE_ENTRY >> 12));
                  getDefaultTimer()->sleep(200);
              }

              while (!doneInit)
                  asm("pause");
              Logger::getLogger("hw").info("CPU [%u] loaded", processorId);
              doneInit = false;
          }
      });

    return true;
}

Process*
SiberixKernel::getKernelProcess()
{
    return &kernelProcess;
}

void
TaskStateSegment::init(GdtPackage* package)
{
    package->tss = GdtTssEntry(*this);

    memset(this, 0, sizeof(TaskStateSegment));

    for (int i = 0; i < 3; i++) {
        ist[i] =
          (u64)siberix()->getMemory().alloc4KPages(8, kernelAddressSpace);
        memset((void*)ist[i], 0, PAGE_SIZE_4K);
        ist[i] += PAGE_SIZE_4K * 8;
    }

    asm volatile("mov %%rsp, %0" : "=r"(rsp[0]));
    asm volatile("ltr %%ax" ::"a"(0x28));
}

void
Logger::printStackTrace(void* _registers)
{
    if (_registers == nullptr) {
        return;
    }
    RegisterContext* registers = reinterpret_cast<RegisterContext*>(_registers);

    log(LOG_INFO,
        "\n(hex)rax=0x%x\n(hex)rbx=0x%x\n(hex)rcx=0x%x\n(hex)rdx=0x%x\n(hex)"
        "rsi=0x%x\n(hex)rdi=0x%x\n(hex)rsp=0x%x\n(hex)rbp=0x%x\n(bin)rflags=%"
        "b\n(oct)cs=%u\n(oct)ss=%u\n",
        registers->rax,
        registers->rbx,
        registers->rcx,
        registers->rdx,
        registers->rsi,
        registers->rdi,
        registers->rsp,
        registers->rbp,
        registers->rflags,
        registers->cs,
        registers->ss);
}