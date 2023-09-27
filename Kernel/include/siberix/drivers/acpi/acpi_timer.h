#include <siberix/device/types.h>
#include <siberix/drivers/acpi/spec.h>

typedef u32 (*tickreader_t)(u32);

class AcpiTimerDevice : public TimerDevice
{
public:
    AcpiTimerDevice(AcpiFadt* fadt);
    ~AcpiTimerDevice();

    void enable() override {}
    void disable() override {}

    void     sleep(Duration duration) override;
    void     sleep(u64 ms) override;
    Duration time() override;

private:
    u8           m_pmt;
    AcpiAddress* m_xpmt;
    bool         m_is32bitMode;
    u32          m_data;
    tickreader_t m_reader;
};