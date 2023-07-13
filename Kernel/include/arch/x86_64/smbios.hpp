#include <siberix/device/device.hpp>

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

struct Smbios2EntryPoint {
    char epString[4];
    u8   checksum;
    u8   length;
    u8   majorv;
    u8   minorv;
    u16  maxSize;
    u8   revision;
    char fmtarea[5];
    char epString2[5];
    u8   checksum2;
    u16  tableLength;
    u32  tableAddress;
    u16  numOfStructs;
    u8   bcdRevision;
};

struct Smbios3EntryPoint {
    char epString[5];
    u8   checksum;
    u8   length;
    u8   majorv;
    u8   minorv;
    u8   docrev;
    u8   revision;
    u8   __reserved__;
    u32  maxSize;
    u64  tableAddress;
};

struct SmbiosHeader {
    u8  type;
    u8  length;
    u32 handle;
};

enum class SmbiosHeaderType : u8 {
    Bios               = 0,
    System             = 1,
    Mainboard          = 2,
    Enclosure          = 3,
    Processor          = 4,
    Cache              = 7,
    Slots              = 9,
    MemoryArray        = 16,
    MemoryDevice       = 17,
    MemoryArrayMapped  = 19,
    MemoryDeviceMapped = 20,
    Boot               = 32
};

struct SmbiosInfoBios {
    u8  vendor;
    u8  version;
    u16 startAddrSegment;
    u8  releaseDate;
    u8  romSize;
    u64 characteristics;
    // Spec. Ver >= 2.4
    u16 charsExtBytes;
    u8  major;
    u8  minor;
    u8  ecfMajor;
    u8  ecfMinor;
    u16 extRomSize;
};

struct SmbiosInfoSystem {};

class SmbiosDevice : public Device {
public:
    SmbiosDevice();
    ~SmbiosDevice();

private:
    int   m_version;
    void* m_entryPoint;

    inline static const char* signL2 = "_SM_";
    inline static const char* signL3 = "_SM3_";
};