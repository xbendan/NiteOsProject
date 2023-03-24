#include <macros>

namespace SMBios
{
    struct Smbios2EntryPoint
    {
        char EntryPointString[4];
        uint8_t Checksum;
        uint8_t Length;
        uint8_t MajorVersion;
        uint8_t MinorVersion;
        uint16_t MaxSize;
        uint8_t Revision;
        char FormatArea[5];
        char EntryPointString2[5];
        uint8_t Checksum2;
        uint16_t TableLength;
        uint32_t TableAddress;
        uint16_t NumberOfStructures;
        uint8_t BCDRevision;
    };

    struct Smbios3EntryPoint
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

    struct SmbiosTypeHeader
    {
        uint8_t Type;
        uint8_t Length;
        uint16_t Handle;
    };

#define BIOS_CHARS_NOT_SUPPORTED (1 << 3)
#define BIOS_CHARS_ISA (1 << 4)
#define BIOS_CHARS_MCA (1 << 5)
#define BIOS_CHARS_EISA (1 << 6)
#define BIOS_CHARS_PCI (1 << 7)
#define BIOS_CHARS_PCMCIA (1 << 8)
#define BIOS_CHARS_PNP (1 << 9) /* Plug and Play */
#define BIOS_CHARS_APM (1 << 10)
#define BIOS_CHARS_IS_UPGRADEABLE (1 << 11)
#define BIOS_CHARS_SHADOWING (1 << 12)
#define BIOS_CHARS_VL_VESA (1 << 13)
#define BIOS_CHARS_ESCD (1 << 14)
#define BIOS_CHARS_BOOT_FROM_CD (1 << 15)
#define BIOS_CHARS_SELECTABLE_BOOT (1 << 16)
#define BIOS_CHARS_ROM_SOCKETED (1 << 17)
#define BIOS_CHARS_BOOT_FROM_PCMCIA (1 << 18)
#define BIOS_CHARS_EDD_SPEC (1 << 19)
#define BIOS_CHARS_JAPFLOPPY_NEC9800 (1 << 20)
#define BIOS_CHARS_JAPFLOPPY_TOSHIBA (1 << 21)
#define BIOS_CHARS_FLOPPY_S525_360 (1 << 22)
#define BIOS_CHARS_FLOPPY_S525_12 (1 << 23)
#define BIOS_CHARS_FLOPPY_S35_720 (1 << 24)
#define BIOS_CHARS_FLOPPY_S35_288 (1 << 25)
#define BIOS_CHARS_PRINT_SCREEN (1 << 26)
#define BIOS_CHARS_8042KEYBOARD (1 << 27)
#define BIOS_CHARS_SERIAL (1 << 28)
#define BIOS_CHARS_PRINTER (1 << 29)
#define BIOS_CHARS_CGA_MONOVIDEO (1 << 30)
#define BIOS_CHARS_NEC_PC98 (1 << 31)

#define BIOS_CHARSEXT_ACPI 1
#define BIOS_CHARSEXT_USB_LEGACY (1 << 1)
#define BIOS_CHARSEXT_AGP (1 << 2)
#define BIOS_CHARSEXT_I2O_BOOT (1 << 3)
#define BIOS_CHARSEXT_LS120_BOOT (1 << 4)
#define BIOS_CHARSEXT_ATAPI_ZIP_BOOT (1 << 5)
#define BIOS_CHARSEXT_1394_BOOT (1 << 6)
#define BIOS_CHARSEXT_SMART_BATTERY (1 << 7)

#define BIOS_CHARSEXTT_BOOT_SPECS 1
/*
 * Function key-initiated network service boot is supported. When function key-uninitiated network 
 * service boot is not supported, a network adapter option ROM may choose to offer this functionality 
 * on its own, thus offering this capability to legacy systems. When the function is supported, the 
 * network adapter option ROM shall not offer this capability.
 */
#define BIOS_CHARSEXTT_FUNC_KEYINIT_NETWORK (1 << 1)
#define BIOS_CHARSEXTT_TARGETED_CONTENT_DIST (1 << 2)
#define BIOS_CHARSEXTT_UEFI_SPECS (1 << 3)
#define BIOS_CHARSEXTT_VIRT_MACHINE (1 << 4)
#define BIOS_CHARSEXTT_MANU_SUPPORTED (1 << 5)
#define BIOS_CHARSEXTT_MANU_ENABLED (1 << 6)

    struct SmbiosBIOSInfomation : public SmbiosTypeHeader
    {
        uint8_t Vendor;
        uint8_t Version;
        uint16_t StartAddrSegment;
        uint8_t ReleaseDate;
        uint8_t RomSize;
        uint64_t Characteristics;
        // Spec. Ver >= 2.4
        uint16_t CharsExtBytes;
        uint8_t MajorRelease;
        uint8_t MinorRelease;
        uint8_t ECFMajorRelease /* Embbedded Controller Firmware */;
        uint8_t ECFMinorRelease;
        uint16_t ExtendedRomSize;
    };

    struct SmbiosSystemInformation : public SmbiosTypeHeader
    {
        
    };
    

    void Initialize();
} // namespace SMBIOS