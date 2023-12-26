#include <stdcxx/types.h>
#include <stdcxx/uuid.h>

namespace Drv::Fs::Ext2 {
    struct ReserveWindow
    {
        uint64_t _start;
        uint64_t _end;
    };

    struct Superblock
    {
        le32_t _inodesCount;
        le32_t _blocksCount;
        le32_t _blocksReservedCount;
        le32_t _blocksFreeCount;
        le32_t _inodesFreeCount;
        le32_t _firstDataBlock;
        le32_t _blockSize;
        le32_t _fragSize;
        le32_t _blocksPerGroup;
        le32_t _fragsPerGroup;
        le32_t _inodesPerGroup;
        le32_t _timeOfMount;
        le32_t _timeOfLastWrite;
        le16_t _mountCount;
        le16_t _maxMountCount;
        le16_t _magic;
        le16_t _state;
        le16_t _errorAction;
        le16_t _minorVersion;
        le32_t _timeOfLastCheck;
        le32_t _checkInterval;
        le32_t _osId;
        le32_t _majorVersion;
        le16_t _uidReserved;
        le16_t _gidReserved;

        le32_t    _firstInode;
        le16_t    _inodeSize;
        le16_t    _blockGroupNumber;
        le32_t    _featureCompat;
        le32_t    _featureIncompat;
        le32_t    _featureRoCompat;
        Std::UUID _uuid;
        char      _volumeName[16];
        char      _lastMounted[64];
        le32_t    _algoBitmapUsage;

        uint8_t  _preallocBlocks;
        uint8_t  _preallocDirBlocks;
        uint16_t _padding1;

        Std::UUID _journalUuid;
        uint32_t  _journalInode;
        uint32_t  _journalDevice;
        uint32_t  _orphanInodeListHead;
        uint32_t  _hashSeed[4];
        uint8_t   _defHashVersion;
        uint8_t   _padding2[3];
        le32_t    _defaultMountOptions;
        le32_t    _firstMetaBlockGroup;
        le32_t    _reserved[190];
    };

    struct GroupDesc
    {
        le32_t _blockBitmap;
        le32_t _inodeBitmap;
        le32_t _inodeTable;
        le16_t _freeBlocksCount;
        le16_t _freeInodesCount;
        le16_t _directoriesCount;
        le16_t _pad;
        le32_t _reserved[3];
    };

    struct Inode
    {
        le16_t _mode;
        le16_t _bitsUid;
        le32_t _size;
        le32_t _timeOfAccess;
        le32_t _timeOfCreation;
        le32_t _timeOfModification;
        le32_t _timeOfDeletion;
        le16_t _bitsGroupId;
        le16_t _linksCount;
        le32_t _blocksCount;
        le32_t _flags;

        le32_t _osd1;

        le32_t _blocks[15];
        le32_t _fileVersion;
        le32_t _fileAcl;
        le32_t _dirAcl;
        le32_t _fragmentAddress;
        struct
        {
            uint8_t  _fragNum;
            uint8_t  _fragSize;
            uint16_t _pad;
            le16_t   _uidHigh;
            le16_t   _gidHigh;
            uint32_t _reserved;
        };
    };
}