#include <siberix/drivers/acpi/spec.h>

#include <siberix/core/time.hpp>

class AcpiTimerDevice : public TimerDevice {
public:
    AcpiTimerDevice();
    ~AcpiTimerDevice();

    void sleep(Duration duration) override;
    void sleep(u32 ms) override;

private:
    u8           m_timerLength;
    AcpiAddress* m_xpmtAddress;
};