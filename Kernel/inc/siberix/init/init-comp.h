#pragma once

#include <siberix/dvc/connectivity.h>
#include <siberix/mem/mem.h>
#include <siberix/proc/sched.h>

namespace Kern::Init {
    void                setupPhysMemory();
    Mem::AddressSpace*  setupVirtMemory();
    Task::ITaskHost*    setupTask();
    DeviceConnectivity* setupDevices();
}