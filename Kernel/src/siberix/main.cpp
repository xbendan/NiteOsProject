#include <siberix/hwtypes.h>
#include <siberix/init/init-arch.h>
#include <siberix/init/init-comp.h>
#include <siberix/main.h>
#include <siberix/proc/svchost.h>
#include <siberix/time/clocksource.h>
#include <siberix/time/timersource.h>
#include <xtra-logging/logger.h>

const char* splash[] = {
    " _____  _  _                  _                            ",
    "/  ___|(_)| |                (_)                           ",
    "\\ `--.  _ | |__    ___  _ __  _ __  __                    ",
    " `--. \\| || '_ \\  / _ \\| '__|| |\\ \\/ /                ",
    "/\\__/ /| || |_) ||  __/| |   | | >  <                     ",
    "\\____/ |_||_.__/  \\___||_|   |_|/_/\\_\\                 "
};

namespace Kern::Main {
    bool                   isInitialized = false;
    Init::BootConfigTable* _bootConfig;

    Svc::MemSvcHost*       svcMem;
    Svc::TaskSvcHost*      svcTask;
    Logger*                logger;
    DeviceConnectivity*    connectivity;
    Io::VirtualFileSystem* fileSystem;

    uint64_t alloc4KPages(uint64_t amount)
    {
        return svcMem->alloc4KPages(amount);
    }

    uint64_t alloc4KPages(uint64_t amount, Mem::AddressSpace* addressSpace)
    {
        return svcMem->alloc4KPages(amount, addressSpace);
    }

    void free4KPages(uint64_t address, uint64_t amount)
    {
        svcMem->free4KPages((void*)address, amount);
    }

    RefPtr<Task::Process> createProcess(Std::String<Utf8> name)
    {
        return svcTask->getProcessFactory()->createProcess(name);
    }

    RefPtr<Task::Process> createIdleProcess()
    {
        return svcTask->getProcessFactory()->createIdleProcess();
    }

    template <typename... Args>
    RefPtr<Task::Process> createProcessEx( //
      Std::String<Utf8> name,
      Task::ProcessType type,
      Io::File*         file,
      Io::Directory*    workingDirectory,
      Args... launchArgs)
    {
        return svcTask->getProcessFactory()->createProcessEx(
          name,
          type,
          file,
          workingDirectory,
          Std::forward<decltype(launchArgs)>(launchArgs)...);
    }

    Device* findDevice(Std::String<Utf8> name)
    {
        return connectivity->findDevice(name);
    }

    Device* findDevice(Std::UUID uuid)
    {
        return connectivity->findDevice(uuid);
    }

    void registerDevice(Device* device)
    {
        connectivity->registerDevice(device);
    }

    void unregisterDevice(Device* device)
    {
        connectivity->unregisterDevice(device);
    }

    Io::RootFsNode* getFsRoot()
    {
        return fileSystem->getRoot();
    }

    Init::BootConfigTable* getBootConfig()
    {
        return _bootConfig;
    }

    using namespace Init;

    [[noreturn]] void setupKernel(Init::BootConfigTable* bootConfig)
    {
        _bootConfig = bootConfig;
        setupArch();

        while (true)
            asm volatile("pause; hlt;");
    }
}