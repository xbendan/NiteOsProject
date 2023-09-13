#include <arch/x86_64/iopt.h>
#include <siberix/core/power.h>

void PowerEngine::shutdown() { outWord16(0x604, 0x2000); }