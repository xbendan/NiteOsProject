#include <siberix/kmain.h>

const char* splash[] = {
    " _____  _  _                  _                            ",
    "/  ___|(_)| |                (_)                           ",
    "\\ `--.  _ | |__    ___  _ __  _ __  __                    ",
    " `--. \\| || '_ \\  / _ \\| '__|| |\\ \\/ /                ",
    "/\\__/ /| || |_) ||  __/| |   | | >  <                     ",
    "\\____/ |_||_.__/  \\___||_|   |_|/_/\\_\\                 "
};

void
kmain(Kern::Init::BootConfigTable& bootConfig)
{
}

namespace Kern::Main {
    bool                   isInitialized = false;
    Task::IProcessHost*    hostedProcess;
    Mem::IMemHost*         hostedMemory;
    DeviceHost*            hostedDevices;
    Io::VirtualFileSystem* fileSystem;

    uint64_t alloc4KPages(uint64_t amount)
    {
        return hostedMemory->alloc4KPages(amount);
    }

    uint64_t alloc4KPages(uint64_t amount, Mem::AddressSpace* addressSpace)
    {
        return hostedMemory->alloc4KPages(amount, addressSpace);
    }

    void free4KPages(uint64_t address, uint64_t amount)
    {
        hostedMemory->free4KPages((void*)address, amount);
    }

    RefPtr<Task::Process> createProcess(Std::String<Utf8> name)
    {
        return hostedProcess->getProcessFactory()->createProcess(name);
    }

    RefPtr<Task::Process> createIdleProcess()
    {
        return hostedProcess->getProcessFactory()->createIdleProcess();
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
}