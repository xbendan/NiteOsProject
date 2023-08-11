#include <common/typedefs.h>
#include <arch/x86_64/types.h>

extern u64 isrTables[];
extern IdtEntry idtEntryList[IDT_ENTRY_COUNT];

enum IntType {
    IntTypeFault = 0x1,
    IntTypeTrap = 0x2,
    IntTypeInterrupt = 0x4,
    IntTypeAbort = 0x8
};

void (*irq_t)(RegisterContext* context);

struct InterruptData {
    const char* name;
    u8 type;
    bool hasErrCode;
    irq_t handler;

    inline bool isFault() { return (type & IntTypeFault); }
    inline bool isTrap() { return (type & IntTypeTrap); }
    inline bool isInterrupt() { return (type & IntTypeInterrupt); }
    inline bool hasErrorCode() { return hasErrCode; }
};