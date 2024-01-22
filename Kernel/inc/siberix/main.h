#include <siberix/dvc/connectivity.h>
#include <siberix/fs/vfs.h>
#include <siberix/init/boot-info.h>
#include <siberix/mem/svchost.h>
#include <siberix/proc/svchost.h>

extern "C" [[noreturn]] void
kmain(Kern::Init::BootConfigTable& bootConfig);

namespace Kern::Main {
    uint64_t               alloc4KPages(uint64_t amount);
    uint64_t               alloc4KPages(uint64_t           amount,
                                        Mem::AddressSpace* addressSpace,
                                        Mem::Page4K**      pageReference);
    void                   free4KPages(uint64_t address, uint64_t amount);
    RefPtr<Task::Process>  createProcess(Std::String<Utf8> name);
    RefPtr<Task::Process>  createIdleProcess();
    RefPtr<Task::Process>  createProcessEx(Std::String<Utf8> name,
                                           Io::File*         file,
                                           Io::Directory*    workingDirectory,
                                           Std::Array<Std::String<Utf8>> args);
    Device*               findDevice(Std::String<Utf8> name);
    Device*               findDevice(Std::UUID uuid);
    void                   registerDevice(Device* device);
    void                   unregisterDevice(Device* device);
    Io::RootFsNode*        getFsRoot();
    Io::FsNode*            getFsNode(Std::String<Utf16> path);
    Io::VirtualFileSystem& vfs();

    Init::BootConfigTable* getBootConfig();
    Svc::MemSvcHost*       getSvcMemory();
    Svc::TaskSvcHost*      getSvcTask();

    [[noreturn]] void main(Init::BootConfigTable& bootConfig);
}