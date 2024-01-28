#pragma once

#include <siberix/dvc/device.h>
#include <siberix/hwtypes.h>
#include <stdcxx/type-traits.h>

namespace Kern {
    class ProcessorDevice : public Device
    {
    public:
        ProcessorDevice(uint32_t processorId, Hal::CPU* cpu)
          : Device("Processor")
          , m_processorId(processorId)
          , m_cpu(cpu)
        {
        }
        ~ProcessorDevice() = default;

        uint32_t  getProcessorId() const { return m_processorId; }
        Hal::CPU* getCPU() const { return m_cpu; }

    private:
        uint32_t  m_processorId;
        Hal::CPU* m_cpu;
    };
}