#include <Kernel>

extern "C" void DispatchInterrupts()
{
    MakeTripleFault();
}

void EnableInterrupts() { asm("sti"); }
void DisableInterrupts() { asm("cli"); }