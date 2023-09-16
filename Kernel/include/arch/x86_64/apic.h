#include <arch/x86_64/arch.h>
#include <siberix/core/time.h>
#include <siberix/device/device.h>
#include <siberix/device/types.h>
#include <siberix/drivers/acpi/spec.h>
#include <utils/array.h>

#define LOCAL_APIC_ID 0x20            // APIC ID Register
#define LOCAL_APIC_VERSION 0x30       // APIC Version Register
#define LOCAL_APIC_TPR 0x80           // Task Priority Register
#define LOCAL_APIC_APR 0x90           // Arbitration Priority Register
#define LOCAL_APIC_PPR 0xA0           // Processor Priority Register
#define LOCAL_APIC_EOI 0xB0           // End of Interrupt Register
#define LOCAL_APIC_RRD 0xC0           // Remote Read Register
#define LOCAL_APIC_LDR 0xD0           // Logical Destination Register
#define LOCAL_APIC_DFR 0xE0           // Destination Format Register
#define LOCAL_APIC_SIVR 0xF0          // Spurious Interrupt Vector Register
#define LOCAL_APIC_ISR 0x100          // In-service Register
#define LOCAL_APIC_TMR 0x180          // Trigger Mode Register
#define LOCAL_APIC_IRR 0x200          // Interrupt Request Register
#define LOCAL_APIC_ERROR_STATUS 0x280 // Error Status Register
#define LOCAL_APIC_ICR_LOW 0x300      // Interrupt Command Register Low
#define LOCAL_APIC_ICR_HIGH 0x310     // Interrupt Command Register High
#define LOCAL_APIC_LVT_TIMER 0x320    // Timer Local Vector Table Entry
#define LOCAL_APIC_LVT_THERMAL 0x330  // Thermal Local Vector Table Entry
#define LOCAL_APIC_LVT_PERF_MONITORING                                         \
    0x340                          // Performance Local Vector Table Entry
#define LOCAL_APIC_LVT_LINT0 0x350 // Local Interrupt 0 Local Vector Table Entry
#define LOCAL_APIC_LVT_LINT1 0x360 // Local Interrupt 1 Local Vector Table Entry
#define LOCAL_APIC_LVT_ERROR 0x370 // Error Local Vector Table Entry
#define LOCAL_APIC_TIMER_INITIAL_COUNT 0x380 // Timer Initial Count Register
#define LOCAL_APIC_TIMER_CURRENT_COUNT 0x390 // Timer Current Count Register
#define LOCAL_APIC_TIMER_DIVIDE 0x3E0 // Timer Divide Configuration Register
#define LOCAL_APIC_ICR_PENDING 0x1000
#define LOCAL_APIC_NMI ((4 << 8))

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
#define IO_APIC_WIN 0x10    // I/O APIC I/O Window Address offset

#define IO_APIC_REGISTER_ID 0x0      // ID Register
#define IO_APIC_REGISTER_VER 0x1     // Version Register
#define IO_APIC_REGISTER_ARB 0x2     // I/O APIC Arbitration ID
#define IO_APIC_RED_TABLE_START 0x10 // I/O APIC Redirection Table Start
#define IO_APIC_RED_TABLE_ENT(x) (0x10 + 2 * x)

#define IO_RED_TBL_VECTOR(x) (x & 0xFF)

class ApicDevice;

class ApicLocalInterface
{
public:
    ApicLocalInterface();
    ApicLocalInterface(u8 _apicId, ApicDevice* _apic);
    ~ApicLocalInterface();

    void sendInterrupt(u32 vector);
    void sendInterrupt(u32 dsh, u32 type, u8 vector);
    void write(u32 reg, u32 data);
    void setup();
    u32  read(u32 reg);
    void eoi();

private:
    u8   apicId;
    u64  basePhys;
    u64  baseVirtIO;
    Cpu* cpu;
};

class ApicTimerDevice : public TimerDevice
{
public:
    ApicTimerDevice(ApicLocalInterface& interface);
    ~ApicTimerDevice();

    void     sleep(Duration duration) override;
    void     sleep(u64 ms) override;
    Duration time() override;

private:
    ApicLocalInterface& m_interface;
    u64                 m_busClock;
    u64                 m_ticks;
};

class ApicDevice : public Device
{
public:
    ApicDevice();
    ~ApicDevice();

    void enable() override;
    void disable() override;

    void                       ioWrite(u32 reg, u32 data);
    u32                        ioRead(u32 reg);
    void                       ioWrite64(u32 reg, u64 data);
    u64                        ioRead64(u32 reg);
    void                       lWriteBase(u64 val);
    u64                        lReadBase();
    inline ApicLocalInterface& getInterface(u8 apicId)
    {
        return m_interfaces[apicId];
    }
    inline ApicLocalInterface& getInterface()
    {
        return m_interfaces[getCpuLocal()->apicId];
    }

private:
    u64           basePhys;
    u64           baseVirtIO;
    volatile u32* ioRegSelect;
    volatile u32* ioWindow;

    static ApicLocalInterface            m_interfaces[256];
    static SizedArrayList<MadtIso*, 256> m_overrides;
};

extern ApicDevice* _apic;