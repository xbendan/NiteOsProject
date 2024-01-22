#include <drivers/acpi/spec.h>
#include <drivers/apic/spec.h>
#include <siberix/dvc/device.h>
#include <siberix/dvc/type-processor.h>
#include <siberix/hwtypes.h>
#include <stdcxx/linked-list.h>
#include <stdcxx/types.h>

namespace Kern::Hal::Impls {
    using namespace Specs;

    class ApicDevice
      : public SmpSvcHost
      , public Device
    {
    public:
        ApicDevice();
        ~ApicDevice() = default;

        void     ioRegWrite(uint32_t reg, uint32_t data);
        uint32_t ioRegRead(uint32_t reg);
        void     ioRedTblWrite(uint32_t index, uint64_t data);
        uint64_t ioRedTblRead(uint32_t index);

        void     localBaseWrite(uint64_t data);
        uint64_t localBaseRead();
        void     localRegWrite(uint32_t reg, uint32_t data);
        uint32_t localRegRead(uint32_t reg);

        void onLoad() override;
        void onEnable() override;
        void onDisable() override;

        Hal::CPU* current() override;

        class ApicLocal
        {
        public:
            ApicLocal(uint8_t          apicId,
                      ApicDevice*      apic,
                      ProcessorDevice* processor)
              : m_apicId(apicId)
              , m_basePhys(apic->localBaseRead() & LOCAL_APIC_BASE)
              , m_baseVirt(Kern::Mem::copyAsIoAddress(m_basePhys))
              , m_processor(processor)
              , m_cpu(processor->getCPU())
            {
            }
            ~ApicLocal() = default;

            inline void regWrite(uint32_t reg, uint32_t data)
            {
                *((volatile uint32_t*)(m_baseVirt + reg)) = data;
            }

            inline uint32_t regRead(uint32_t reg)
            {
                return *((volatile uint32_t*)(m_baseVirt + reg));
            }

            void sendIpi(uint32_t vector)
            {
                regWrite(LOCAL_APIC_ICR_HIGH, ((uint32_t)m_apicId) << 24);
                regWrite(LOCAL_APIC_ICR_LOW, vector);

                while ((regRead((uint32_t)LOCAL_APIC_ICR_LOW) &
                        LOCAL_APIC_ICR_PENDING) != 0)
                    ;
            }

            void sendIpi(uint32_t dsh, uint32_t type, uint8_t vector)
            {
                sendIpi(dsh | type | ICR_VECTOR(vector));
            }

            uint8_t getApicId() const { return m_apicId; }

        private:
            uint8_t          m_apicId;
            uint64_t         m_basePhys;
            uint64_t         m_baseVirt;
            ProcessorDevice* m_processor;
            Hal::CPU*        m_cpu;
        };

    private:
        uint64_t                     m_ioBasePhys;
        uint64_t                     m_ioBaseVirt;
        uint32_t                     m_interrupts;
        volatile uint32_t*           m_ioRegSel;
        volatile uint32_t*           m_ioWindow;
        Std::LinkedList<ApicLocal*>* m_interfaces;
        Std::LinkedList<MadtIso*>*   m_overrides;
    };
}