#include <drivers/pci/device.h>

namespace Kern::Hal::Impls {
    struct NvmeControllerRegs
    {
        uint64_t _capabilities;
        uint32_t _version;
        uint32_t _interruptMaskSet;
        uint32_t _interruptMaskClear;
        uint32_t _config;
        uint32_t _reserved0;
        uint32_t _status;
        uint32_t _nvmSubsystemReset;
        uint32_t _adminQueueAttr;
        uint64_t _adminQueueSubmission;
        uint64_t _adminQueueCompletion;
        uint32_t _memBufferLocation;
        uint32_t _memBufferSize;
        uint32_t _bootPartInfo;
        uint32_t _bootPartReadSel;
        uint32_t _bootPartMemBufferLoc;
        uint32_t _bootPartMemBufferSize;
        uint64_t _memBufferSpaceControl;
        uint32_t _memBufferStatus;
    } __attribute__((packed));

    class NvmeControllerDevice : public IPeriCompDevice
    {};
}