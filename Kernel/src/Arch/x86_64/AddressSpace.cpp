#include <Mem/AddressSpace.h>
#include <Mem/Page.h>

#include <Arch/x86_64/MMU.h>

AddressSpace::AddressSpace()
  : m_AllocatedPages(0),
    m_MappedPages(0),
    m_ZeroPage(Memory::AllocatePhysMemory4K(1)),
    m_VirtualPages(new Paging::VirtualPages())
{

}

AddressSpace::~AddressSpace()
{

}

uintptr_t AddressSpace::Allocate4KPages(size_t amount)
{
    
}

void AddressSpace::Free4KPages(uintptr_t address, size_t amount)
{

}

uintptr_t AddressSpace::Allocate4KPages(size_t amount)
{

}

void AddressSpace::Free4KPages(uintptr_t address, size_t amount)
{

}