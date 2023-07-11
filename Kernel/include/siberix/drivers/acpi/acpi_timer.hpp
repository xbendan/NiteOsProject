#include <siberix/drivers/acpi/spec.h>

#include <siberix/core/time.hpp>

typedef u32 (*tickreader_t)(u32);

class AcpiTimerDevice : public TimerDevice {
public:
    AcpiTimerDevice();
    ~AcpiTimerDevice();

    void sleep(Duration duration) override;
    void sleep(u64 ms) override;

private:
    u8            m_timerLength;
    AcpiAddress*  m_xpmtAddress;
    bool          m_is32bitMode;
    u32           m_data;
    tickreader_t* m_tickReader;
};