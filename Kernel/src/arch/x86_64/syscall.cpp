#include <arch/x86_64/types.h>

long sysFileRead(RegisterContext* regs);
long sysFileWrite(RegisterContext* regs);
long sysFileOpen(RegisterContext* regs);
long sysFileClose(RegisterContext* regs);
long sysFileCreate(RegisterContext* regs);
long sysFileDelete(RegisterContext* regs);
long sysFileRename(RegisterContext* regs);