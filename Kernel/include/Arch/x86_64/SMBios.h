#include <macros.h>

namespace Firmware::SMBios
{
    struct SMBIOS2EntryPoint
    {
        char EntryPointString[4];
        uint8_t Checksum;
        uint8_t Length;
        uint8_t MajorVersion;
        uint8_t MinorVersion;
        uint16_t MaxSize;
        uint8_t Revision;
        char FmtArea[5];
        char EntryPointString2[5];
        uint8_t Checksum2;
        uint16_t TableLength;
        uint32_t TableAddress;
        uint16_t NumberOfStructures;
        uint8_t BCDRevision;
    };

    struct SMBIOS3EntryPoint
    {
        char EntryPointString[5];
        uint8_t Checksum;
        uint8_t Length;
        uint8_t MajorVersion;
        uint8_t MinorVersion;
        uint8_t Docrev;
        uint8_t Revision;
        uint8_t __reserved__0;
        uint32_t MaxSize;
        uint64_t TableAddress;
    };

    struct SMBIOSHeader
    {
        uint8_t Type;
        uint8_t Length;
        uint16_t Handle;
    };

    void Initialize();
} // namespace SMBIOS