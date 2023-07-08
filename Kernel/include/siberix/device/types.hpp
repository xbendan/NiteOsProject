#include <siberix/device/device.hpp>

class ProcessorDevice : public Device {
public:
    ProcessorDevice(u32 _processorId);
    ~ProcessorDevice();

private:
    u32 processorId;
};