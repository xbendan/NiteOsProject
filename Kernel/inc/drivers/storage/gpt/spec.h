#include <stdcxx/types.h>
#include <stdcxx/uuid.h>

namespace Kern::Hal::Specs {
    struct GuidPartTable
    {
        uint64_t  _signature;
        uint32_t  _revision;
        uint32_t  _headerSize;
        uint32_t  _crc32Header;
        uint32_t  _reserved0;
        uint64_t  _currentLba;
        uint64_t  _backupLba;
        uint64_t  _firstUsableLba;
        uint64_t  _lastUsableLba;
        Std::UUID _guid;
        uint64_t  _partitionEntryLba;
        uint32_t  _numPartitionEntries;
        uint32_t  _partitionEntrySize;
        uint32_t  _crc32PartitionArray;
    } __attribute__((packed));

    struct GuidPartEntry
    {
        Std::UUID _partitionTypeGuid;
        Std::UUID _uniquePartitionGuid;
        uint64_t  _startingLba;
        uint64_t  _endingLba;
        uint64_t  _attributes;
        uint16_t  _name[36];
    } __attribute__((packed));
}