#include <drivers/acpi/spec.h>
#include <siberix/dvc/device.h>
#include <stdcxx/type-traits.h>

namespace Kern::Hal {
    class AcpiPmDevice : public Device
    {
    public:
        AcpiPmDevice();
        ~AcpiPmDevice();

        template <typename T>
            requires Std::IsBaseOf<AcpiTable, T>::Value
        T* findTable(Std::String<Utf8> name, int index = 0);

    private:
        AcpiRsdp* m_rsdp;
        AcpiRsdt* m_rsdt;
        AcpiXsdp* m_xsdp;
        AcpiXsdt* m_xsdt;
        Madt*     m_madt;
        Hpet*     m_hpet;
        AcpiFadt* m_fadt;
        PciMcfg*  m_mcfg;
    };
}