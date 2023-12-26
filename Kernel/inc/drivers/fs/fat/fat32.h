#include <stdcxx/types.h>

namespace Drv::Fs::Fat32 {
    struct BiosParamBlock
    {
        uint16_t _bytesPerSector;
        uint8_t  _sectorsPerCluster;
        uint16_t _reservedSectors;
        uint8_t  _fatCount;
        uint16_t _rootEntryCount;
        uint16_t _totalSectors16;
        uint8_t  _media;
        uint16_t _sectorsPerFat16;
        uint16_t _sectorsPerTrack;
        uint16_t _headCount;
        uint32_t _hiddenSectors;
        uint32_t _totalSectors32;
        uint32_t _sectorsPerFat32;
        union
        {
            uint16_t _extFlags;
            struct
            {
                uint16_t _activeFat : 4;
                uint16_t _reserved1 : 3;
                uint16_t _fatMirrored : 1;
                uint16_t _reserved2 : 8;
            };
        };
        uint16_t _version;
        uint32_t _rootCluster;
        uint16_t _fsInfo;
        uint16_t _backupBootSector;
    };

    struct ExtBootRecord
    {
        uint32_t _sectorsPerFat;
        uint16_t _flags;
        uint16_t _version;
        uint32_t _rootCluster;
        uint16_t _fsInfo;
        uint16_t _backupBootSector;
        uint8_t  _reserved[12];
        uint8_t  _driveNumber;
        uint8_t  _reserved1;
        uint8_t  _bootSignature;
        uint32_t _volumeId;
        uint8_t  _volumeLabel[11];
        uint8_t  _fileSystemType[8];
    };

    struct FsInfo
    {
        uint32_t _leadSignature;
        uint8_t  _reserved1[480];
        uint32_t _structSignature;
        uint32_t _freeClusterCount;
        uint32_t _nextFreeCluster;
        uint8_t  _reserved2[12];
        uint32_t _trailSignature;
    };

    struct BootRecord
    {
        BiosParamBlock _biosParamBlock;
        ExtBootRecord  _ext;
        FsInfo         _fsInfo;
    };

    struct Entry
    {
        uint8_t  _filename[8];
        uint8_t  _extension[3];
        uint8_t  _attributes;
        uint8_t  _reserved;
        uint8_t  _creationTimeTenths;
        uint16_t _creationTime;
        uint16_t _creationDate;
        uint16_t _lastAccessDate;
        uint16_t _firstClusterHigh;
        uint16_t _lastWriteTime;
        uint16_t _lastWriteDate;
        uint16_t _firstClusterLow;
        uint32_t _fileSize;
    };

    struct LongEntry
    {
        uint8_t  _ord;
        uint16_t _name0[5];
        uint8_t  _attributes;
        uint8_t  _type;
        uint8_t  _checksum;
        uint16_t _name1[6];
        uint16_t _firstCluster;
        uint16_t _name2[2];
    };
}