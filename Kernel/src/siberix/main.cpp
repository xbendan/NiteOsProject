#include <siberix/hwtypes.h>
#include <siberix/init/init-arch.h>
#include <siberix/init/init-comp.h>
#include <siberix/init/init-drvs.h>
#include <siberix/main.h>
#include <siberix/time/clocksource.h>
#include <siberix/time/timersource.h>

const char* splash[] = {
    " _____  _  _                  _                            ",
    "/  ___|(_)| |                (_)                           ",
    "\\ `--.  _ | |__    ___  _ __  _ __  __                    ",
    " `--. \\| || '_ \\  / _ \\| '__|| |\\ \\/ /                ",
    "/\\__/ /| || |_) ||  __/| |   | | >  <                     ",
    "\\____/ |_||_.__/  \\___||_|   |_|/_/\\_\\                 "
};

namespace Kern::Main {
    bool isInitialized = false;
    BootConfigTable & _bootConfig;

    Task::ITaskHost        hostedProcess;
    Mem::IMemoryHost       hostedMemory;
    DeviceConnectivity*    hostedDevices;
    Io::VirtualFileSystem* fileSystem;

    uint64_t alloc4KPages(uint64_t amount)
    {
        return hostedMemory.alloc4KPages(amount);
    }

    uint64_t alloc4KPages(uint64_t amount, Mem::AddressSpace* addressSpace)
    {
        return hostedMemory.alloc4KPages(amount, addressSpace);
    }

    void free4KPages(uint64_t address, uint64_t amount)
    {
        hostedMemory.free4KPages((void*)address, amount);
    }

    RefPtr<Task::Process> createProcess(Std::String<Utf8> name)
    {
        return hostedProcess.getProcessFactory()->createProcess(name);
    }

    RefPtr<Task::Process> createIdleProcess()
    {
        return hostedProcess.getProcessFactory()->createIdleProcess();
    }

    RefPtr<Task::Process> createProcessEx(
      Std::String<Utf8>              name,
      Io::File*                      file,
      Io::Directory*                 workingDirectory,
      Std::Array<Std::String<Utf8>>* launchArgs)
    {
        return hostedProcess->getProcessFactory()->createProcessEx(
          name, file, workingDirectory, launchArgs);
    }

    Io::RootFsNode& getFsRoot()
    {
        return fileSystem->getRoot();
    }

    Init::BootConfigTable* getBootConfig()
    {
        return _bootConfig;
    }

    using namespace Init;

    [[noreturn]] void main(Init::BootConfigTable& bootConfig)
    {
        _bootConfig = bootConfig;
        setupArch();
        hostedMemory = Mem::IMemoryHost();
    }
}