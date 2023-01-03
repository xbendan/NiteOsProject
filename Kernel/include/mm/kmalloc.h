extern "C" void *kmalloc(size_t size);
extern "C" void kfree(void *ptr);

extern "C" uint64_t kallocpg(size_t amount);
extern "C" void kfreepg(uintptr_t addr);
extern "C" uint64_t allocpg(size_t amount);
extern "C" void freepg(uintptr_t addr);