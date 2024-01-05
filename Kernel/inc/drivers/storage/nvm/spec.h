#include <stdcxx/types.h>

#define NVME_CAP_CMBS (1 << 57)          // Controller memory buffer supported
#define NVME_CAP_PMRS (1 << 56)          // Persistent memory region supported
#define NVME_CAP_BPS (1 << 45)           // Boot partition support
#define NVME_CAP_NVM_CMD_SET (1UL << 37) // NVM command set supported
#define NVME_CAP_NSSRS (1UL << 36)       // NVM subsystem reset supported
#define NVME_CAP_CQR (1 << 16)           // Contiguous Queues Required

#define NVME_CAP_MPS_MASK 0xfU
#define NVME_CAP_MPSMAX(x)                                                     \
    ((x >> 52) &                                                               \
     NVME_CAP_MPS_MASK) // Max supported memory page size (2 ^ (12 + MPSMAX))
#define NVME_CAP_MPSMIN(x)                                                     \
    ((x >> 48) &                                                               \
     NVME_CAP_MPS_MASK) // Min supported memory page size (2 ^ (12 + MPSMIN))

#define NVME_CAP_DSTRD_MASK 0xfU
#define NVME_CAP_DSTRD(x)                                                      \
    (((x) >> 32) &                                                             \
     NVME_CAP_DSTRD_MASK) // Doorbell stride (2 ^ (2 + DSTRD)) bytes

#define NVME_CAP_MQES_MASK 0xffffU
#define NVME_CAP_MQES(x)                                                       \
    ((x) & NVME_CAP_MQES_MASK) // Maximum queue entries supported

#define NVME_CFG_MPS_MASK 0xfUL
#define NVME_CFG_MPS(x)                                                        \
    (((x) & NVME_CFG_MPS_MASK)                                                 \
     << 7)                       // Host memory page size (2 ^ (12 + MPSMIN))
#define NVME_CFG_CSS_MASK 0b111U // Command set selected
#define NVME_CFG_CSS(x) (((x) & NVME_CFG_CSS_MASK) << 4)
#define NVME_CFG_ENABLE (1 << 0)

#define NVME_CFG_DEFAULT_IOCQES (4 << 20) // 16 bytes so log2(16) = 4
#define NVME_CFG_DEFAULT_IOSQES (6 << 16) // 64 bytes so log2(64) = 6

#define NVME_CSTS_FATAL (1 << 1)
#define NVME_CSTS_READY                                                        \
    (1 << 0) // Set to 1 when the controller is ready to accept submission queue
             // doorbell writes
#define NVME_CSTS_NSSRO (1 << 4) // NVM Subsystem reset occurred

#define NVME_AQA_AQS_MASK 0xfffU // Admin queue size mask
#define NVME_AQA_ACQS(x)                                                       \
    (((x) & NVME_AQA_AQS_MASK) << 16) // Admin completion queue size
#define NVME_AQA_ASQS(x)                                                       \
    (((x) & NVME_AQA_AQS_MASK) << 0) // Admin submission queue size

#define NVME_NSSR_RESET_VALUE 0x4E564D65 // "NVME", initiates a reset

namespace Kern::Hal::Specs {

    struct NvmeCommandBase
    {
        struct
        {
            uint32_t _opcode : 8;
            uint32_t _fuse : 2;
            uint32_t _reserved0 : 4;
            uint32_t _psdt : 2;
            uint32_t _cid : 16;
        } __attribute__((packed));
        uint32_t _nsid;
        uint64_t _reserved1;
        uint64_t _mptr;
        uint64_t _prp1;
        uint64_t _prp2;
    };

    struct NvmeIdentify : public NvmeCommandBase
    {
        enum
        {
            CNS_NAMESPACE         = 0,
            CNS_CONTROLLER        = 1,
            CNS_ACTIVE_NAMESPACES = 2,
        };

        struct
        {
            uint32_t _cns;
            uint32_t _reserved0;
            uint64_t _nsid;
        } __attribute__((packed));

        uint32_t _nvmSetId;
    };

    struct NvmeCreateIoCompletionQueue : public NvmeCommandBase
    {
        struct
        {
            uint32_t _queueId : 16;
            uint32_t _queueSize : 16;
        } __attribute__((packed));

        struct
        {
            uint32_t _contiguous : 1;
            uint32_t _withInterrupt : 1;
            uint32_t _reserved0 : 14;
            uint32_t _intVector : 16;
        } __attribute__((packed));
    };

    struct NvmeCreateIoSubmissionQueue : public NvmeCommandBase
    {
        struct
        {
            uint32_t _queueId : 16;
            uint32_t _queueSize : 16;
        } __attribute__((packed));

        struct
        {
            uint32_t _contiguous : 1;
            uint32_t _priority : 2;
            uint32_t _reserved0 : 13;
            uint32_t _cqId : 16;
        } __attribute__((packed));
    };

    struct NvmeDeleteIoCompletionQueue : public NvmeCommandBase
    {
        struct
        {
            uint32_t _queueId : 16;
            uint32_t _reserved0 : 16;
        } __attribute__((packed));
    };

    struct NvmeSetFeatures
    {
        enum
        {
            FeatureIdNumerOfQueues = 0x7,
        };
        struct
        {
            uint32_t _featId : 8;
            uint32_t _reserved0 : 23;
            uint32_t _save : 1;
        } __attribute__((Packed));
        uint32_t _dw11;
        uint32_t _dw12;
        uint32_t _dw13;
    };

    struct NvmeWrite
    {
        uint64_t _startingLba;
        struct
        {
            uint32_t _blockNum : 16;
            uint32_t _reserved0 : 4;
            uint32_t _drType : 4;
            uint32_t _reserved1 : 2;
            uint32_t _prInfo : 4;
            uint32_t _forceAccess : 1;
            uint32_t _limitedRetry : 1;
        };
    };

    struct NvmeRead : public NvmeWrite
    {
        uint64_t _startingLba;
        struct
        {
            uint32_t _blockNum : 16;
            uint32_t _reserved0 : 10;
            uint32_t _prInfo : 4;
            uint32_t _forceAccess : 1;
            uint32_t _limitedRetry : 1;
        };
    };

}