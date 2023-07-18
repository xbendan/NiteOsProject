#include <siberix/device/device.hpp>

class ProcessorDevice : public Device {
public:
    ProcessorDevice(u32 _processorId);
    ~ProcessorDevice();

private:
    ProcessorDevice *m_self;
    u32              m_processorId;
};

class VisualOutputDevice : public Device {};