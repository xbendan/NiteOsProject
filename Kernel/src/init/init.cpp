#include <common/arch.h>
#include <siberix/mm/page.hpp>

static Memory::SegAlloc _segAlloc;
static Memory::BuddyAlloc _buddyAlloc;

SystemRuntime::loadMemory() {
    this->pageAlloc = &(_segAlloc = Memory::SegAlloc());
}