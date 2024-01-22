#include <stdcxx/types.h>

namespace Kern::Platform::X64 {
    struct Smbios2EntryPoint
    {
        char     _signature[4];
        uint8_t  _checksum;
        uint8_t  _length;
        uint8_t  _majorVer;
        uint8_t  _minorVer;
        uint16_t _maxSize;
        uint8_t  _revision;
        char     _formatArea[5];
        char     _signature2[5];
        uint8_t  _checksum2;
        uint16_t _tableLength;
        uint32_t _tableAddress;
        uint16_t _structNums;
        uint8_t  _bcdRevision;
    };

    struct Smbios3EntryPoint
    {
        char     _signature[5];
        uint8_t  _checksum;
        uint8_t  _length;
        uint8_t  _majorVer;
        uint8_t  _minorVer;
        uint8_t  _docrev;
        uint8_t  _revision;
        uint8_t  _reserved0;
        uint32_t _maxSize;
        uint64_t _tableAddress;
    };

    struct SmbiosHeader
    {
        uint8_t  _type;
        uint8_t  _length;
        uint16_t _handle;
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
 * Function key-initiated network service boot is supported. When function
 * key-uninitiated network service boot is not supported, a network adapter
 * option ROM may choose to offer this functionality on its own, thus offering
 * this capability to legacy systems. When the function is supported, the
 * network adapter option ROM shall not offer this capability.
 */
#define BIOS_CHARSEXTT_FUNC_KEYINIT_NETWORK (1 << 1)
#define BIOS_CHARSEXTT_TARGETED_CONTENT_DIST (1 << 2)
#define BIOS_CHARSEXTT_UEFI_SPECS (1 << 3)
#define BIOS_CHARSEXTT_VIRT_MACHINE (1 << 4)
#define BIOS_CHARSEXTT_MANU_SUPPORTED (1 << 5)
#define BIOS_CHARSEXTT_MANU_ENABLED (1 << 6)

    struct SmbiosBIOSInfo : public SmbiosHeader
    {
        uint8_t  _vendor;
        uint8_t  _version;
        uint16_t _startingAddrSeg;
        uint8_t  _releaseDate;
        uint8_t  _romSize;
        uint64_t _chars;
        // Spec. Ver >= 2.4
        uint16_t _charsExtBytes;
        uint8_t  _majorRel;
        uint8_t  _minorRel;
        uint8_t  _ecfMajorRel;
        uint8_t  _ecfMinorRel;
    };

    struct SmbiosSystemInfo : public SmbiosHeader
    {};

}