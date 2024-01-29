#include <boot-protocols/limine.h>

extern "C" [[noreturn]] void
kload_limine()
{
    while (true)
        asm volatile("cli; hlt");
}