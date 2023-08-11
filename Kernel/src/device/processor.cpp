#include <siberix/device/types.hpp>

ProcessorDevice::ProcessorDevice(u32 _processorId)
    : m_processorId(_processorId),
      Device("Unnamed processor", DeviceBus::Unknown, DeviceType::Processor) {}

ProcessorDevice::~ProcessorDevice() {}