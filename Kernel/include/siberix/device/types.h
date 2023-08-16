#pragma once

#include <siberix/core/time.h>
#include <siberix/device/device.h>

class ProcessorDevice : public Device {
public:
    ProcessorDevice(u32 _processorId);
    ~ProcessorDevice();

private:
    u32 m_processorId;
};

class VisualOutputDevice : public Device {};

class TimerDevice : public Device {
public:
    TimerDevice(const char *name)
        : Device(name, DeviceBus::Software, DeviceType::SystemDevices) {}
    ~TimerDevice() {}

    virtual void sleep(Duration duration) = 0;
    virtual void sleep(u64 ms)            = 0;

    const char *getName() { return m_name; }
    bool        isDefault() { return m_isDefault; }
    void        setDefault(bool isDef) { m_isDefault = isDef; }
    u8          getId() { return m_id; }

private:
    bool m_isDefault;
    u8   m_id;
};