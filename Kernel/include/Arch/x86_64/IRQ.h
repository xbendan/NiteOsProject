#pragma once

#include <Arch/x86_64/CPU.h>

void DisableInterrupts();
void EnableInterrupts();

typedef void (*isr_t)(void*, registers_t*);

void RegisterInterruptHandler(int num, isr_t func);

class InterruptRetainer
{
    InterruptRetainer() { volatile asm("cli"); }
    ~InterruptRetainer() { volatile asm("sti"); }
}