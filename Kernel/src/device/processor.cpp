#include <siberix/device/types.h>

ProcessorDevice::ProcessorDevice(u32 _processorId)
    : Device("Unnamed processor", DeviceBus::Unknown, DeviceType::Processor),
      m_processorId(_processorId) {}

ProcessorDevice::~ProcessorDevice() {}