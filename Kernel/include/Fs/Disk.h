#pragma once

#include <Device/Device.h>
#include <macros>

class DiskDevice : public Device
{
public:
    DiskDevice(const char *name, DeviceBus bus) : Device(name, bus, DeviceType::DeviceTypeDiskDrive) {}
    DiskDevice(const char *name) : DiskDevice(name, DeviceBusPCI) {}

    virtual int Read(uint64_t lba, uint32_t count, void *buffer) = 0;
    virtual int Write(uint64_t lba, uint32_t count, void *buffer) = 0;

    virtual void Enable();
    virtual void Disable();
};