#include <arch/x86_64/types.h>
#include <common/typedefs.h>

extern u64      intTables[];
extern IdtEntry idtEntryList[IDT_ENTRY_COUNT];

enum IntType {
    IntTypeFault     = 0x1,
    IntTypeTrap      = 0x2,
    IntTypeInterrupt = 0x4,
    IntTypeAbort     = 0x8
};

typedef void (*irq_t)(RegisterContext*);

struct InterruptData {
    const char* name;
    u8          type;
    bool        hasErrCode;
    irq_t       handler;

    inline bool isFault() { return (type & IntTypeFault); }
    inline bool isTrap() { return (type & IntTypeTrap); }
    inline bool isInterrupt() { return (type & IntTypeInterrupt); }
    inline bool hasErrorCode() { return hasErrCode; }
};