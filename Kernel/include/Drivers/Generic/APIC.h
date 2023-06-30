#include <Timer.h>
#include <macros>

#define LOCAL_APIC_ENABLE (1 << 10)

#define LOCAL_APIC_ID 0x20 // APIC ID Register
#define LOCAL_APIC_VERSION 0x30 // APIC Version Register
#define LOCAL_APIC_TPR 0x80 // Task Priority Register 
#define LOCAL_APIC_APR 0x90 // Arbitration Priority Register
#define LOCAL_APIC_PPR 0xA0 // Processor Priority Register
#define LOCAL_APIC_EOI 0xB0 // End of Interrupt Register
#define LOCAL_APIC_RRD 0xC0 // Remote Read Register
#define LOCAL_APIC_LDR 0xD0 // Logical Destination Register
#define LOCAL_APIC_DFR 0xE0 // Destination Format Register
#define LOCAL_APIC_SIVR 0xF0 // Spurious Interrupt Vector Register
#define LOCAL_APIC_ISR 0x100 // In-service Register
#define LOCAL_APIC_TMR 0x180 // Trigger Mode Register
#define LOCAL_APIC_IRR 0x200 // Interrupt Request Register
#define LOCAL_APIC_ERROR_STATUS 0x280 // Error Status Register
#define LOCAL_APIC_ICR_LOW 0x300 // Interrupt Command Register Low
#define LOCAL_APIC_ICR_HIGH 0x310 // Interrupt Command Register High
#define LOCAL_APIC_LVT_TIMER 0x320 // Timer Local Vector Table Entry
#define LOCAL_APIC_LVT_THERMAL 0x330 // Thermal Local Vector Table Entry
#define LOCAL_APIC_LVT_PERF_MONITORING 0x340 // Performance Local Vector Table Entry
#define LOCAL_APIC_LVT_LINT0 0x350 // Local Interrupt 0 Local Vector Table Entry
#define LOCAL_APIC_LVT_LINT1 0x360 // Local Interrupt 1 Local Vector Table Entry
#define LOCAL_APIC_LVT_ERROR 0x370 // Error Local Vector Table Entry
#define LOCAL_APIC_TIMER_INITIAL_COUNT 0x380 // Timer Initial Count Register
#define LOCAL_APIC_TIMER_CURRENT_COUNT 0x390 // Timer Current Count Register
#define LOCAL_APIC_TIMER_DIVIDE 0x3E0 // Timer Divide Configuration Register
#define LOCAL_APIC_ICR_PENDING 0x1000
#define LOCAL_APIC_NMI ((4 << 8))

#define LOCAL_APIC_BASE 0xFFFFFFFFFF000

#define ICR_VECTOR(x) (x & 0xFF)
#define ICR_MESSAGE_TYPE_FIXED 0
#define ICR_MESSAGE_TYPE_LOW_PRIORITY (1 << 8)
#define ICR_MESSAGE_TYPE_SMI (2 << 8)
#define ICR_MESSAGE_TYPE_REMOTE_READ (3 << 8)
#define ICR_MESSAGE_TYPE_NMI (4 << 8)
#define ICR_MESSAGE_TYPE_INIT (5 << 8)
#define ICR_MESSAGE_TYPE_STARTUP (6 << 8)
#define ICR_MESSAGE_TYPE_EXTERNAL (7 << 8)

#define ICR_DSH_DEST 0          // Use destination field
#define ICR_DSH_SELF (1 << 18)  // Send to self
#define ICR_DSH_ALL (2 << 18)   // Send to ALL APICs
#define ICR_DSH_OTHER (3 << 18) // Send to all OTHER APICs 

#define IO_APIC_REGSEL 0x00 // I/O APIC Register Select Address Offset
#define IO_APIC_WIN 0x10 // I/O APIC I/O Window Address offset

#define IO_APIC_REGISTER_ID 0x0 // ID Register
#define IO_APIC_REGISTER_VER 0x1 // Version Register
#define IO_APIC_REGISTER_ARB 0x2 // I/O APIC Arbitration ID
#define IO_APIC_RED_TABLE_START 0x10 // I/O APIC Redirection Table Start
#define IO_APIC_RED_TABLE_ENT(x) (0x10 + 2 * x)

#define IO_RED_TBL_VECTOR(x) (x & 0xFF)

namespace APIC
{
    namespace IO
    {
        void WriteData32(uint32_t reg, uint32_t data);
        uint32_t ReadData32(uint32_t reg);
        void WriteData64(uint32_t reg, uint64_t data);
        uint64_t ReadData64(uint32_t reg);
        void Initialize();
        void Redirect(uint8_t irq, uint8_t vector, uint32_t delivery);
        void SetBase(uintptr_t newBase);
        void MapLegacyIRQ(uint8_t irq);
        void EnableInterrupt(uint8_t irq);
    } // namespace IO
    namespace Local
    {
        extern uintptr_t BasePhys;
        extern uintptr_t BaseVirtIO;

        void WriteBase(uint64_t val);
        uint64_t ReadBase();
        void WriteData(uint32_t reg, uint32_t data);
        uint32_t ReadData(uint32_t reg);
        void SendIPI(uint8_t apicId, uint32_t vector);
        void SendIPI(uint8_t apicId, uint32_t dsh, uint32_t type, uint8_t vector);
        void Enable();
        void Initialize();
        void EndOfInterrupt();
    } // namespace Local

    void Initialize();
} // namespace APIC

#include <Arch/x86_64/Interrupts.h>

class LocalAPICTimer : public Timer
{
private:
    uint64_t m_BusClock;
    volatile uint64_t m_Ticks;

public:
    static void TimerEvent(InterruptData *data, RegisterContext *regs);

    LocalAPICTimer();
    ~LocalAPICTimer();

    uint32_t EstimateBusSpeed();
    void Tick() override;
    uint64_t CurrentTime(TimeSpan span = Millisecond) override;
    void Sleep(long milliseconds) override;
};