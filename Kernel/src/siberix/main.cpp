#include <siberix/hwtypes.h>
#include <siberix/init/init-arch.h>
#include <siberix/init/init-comp.h>
#include <siberix/init/init-drvs.h>
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

    RefPtr<Task::Process> createProcessEx(
      Std::String<Utf8>              name,
      Io::File*                      file,
      Io::Directory*                 workingDirectory,
      Std::Array<Std::String<Utf8>>* launchArgs)
    {
        return svcTask->getProcessFactory()->createProcessEx(
          name, file, workingDirectory, launchArgs);
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

    [[noreturn]] void main(Init::BootConfigTable* bootConfig)
    {
        _bootConfig = bootConfig;
        setupArch();

        while (true)
            asm volatile("pause; hlt;");
    }
}