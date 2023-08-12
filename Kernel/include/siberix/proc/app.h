#include <siberix/fs/vfs.h>

#include <siberix/proc/process.h>

struct AppManiest {
    const char *name;
    const char *publisher;
    const char *package;
};

class AppPackage {
public:
    Process *createProcess();

private:
};