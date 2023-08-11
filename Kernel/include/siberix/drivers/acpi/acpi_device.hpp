#include <siberix/device/device.h>
#include <siberix/drivers/acpi/spec.h>

class AcpiPmDevice : public Device {
public:
    AcpiPmDevice();
    ~AcpiPmDevice();

    template <typename T>
    T* findTable(const char* str, int index = 0);

    void enable() override;
    void disable() override;

    AcpiRsdp* rsdp;
    AcpiRsdt* rsdt;
    AcpiXsdp* xsdp;
    AcpiXsdt* xsdt;
    Madt* madt;
    Hpet* hpet;
    AcpiFadt* fadt;
    PciMcfg* mcfg;
};