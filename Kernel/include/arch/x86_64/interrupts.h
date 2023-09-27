#include <arch/x86_64/types.h>
#include <common/typedefs.h>

extern u64      intTables[];
extern IdtEntry idtEntryList[IDT_ENTRY_COUNT];

enum IntType
{
    IntTypeFault     = 0x1,
    IntTypeTrap      = 0x2,
    IntTypeInterrupt = 0x4,
    IntTypeAbort     = 0x8
};

typedef void (*irq_t)(RegisterContext*);

struct Interrupt
{
    const char* m_name;
    u8          m_type;
    bool        m_hasErrCode;
    irq_t       m_executor;

    inline bool isFault() { return (m_type & IntTypeFault); }
    inline bool isTrap() { return (m_type & IntTypeTrap); }
    inline bool isInterrupt() { return (m_type & IntTypeInterrupt); }
    inline bool hasErrorCode() { return m_hasErrCode; }
    inline void setExecutor(irq_t executor) { m_executor = executor; }
};