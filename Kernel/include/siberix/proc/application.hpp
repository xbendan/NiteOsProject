#include <siberix/fs/vfs.hpp>

class Process;

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