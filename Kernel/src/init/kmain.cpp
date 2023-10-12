#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/init/boot.h>

static const char* splash[] = {
    " _____  _  _                  _                            ",
    "/  ___|(_)| |                (_)                           ",
    "\\ `--.  _ | |__    ___  _ __  _ __  __                    ",
    " `--. \\| || '_ \\  / _ \\| '__|| |\\ \\/ /                ",
    "/\\__/ /| || |_) ||  __/| |   | | >  <                     ",
    "\\____/ |_||_.__/  \\___||_|   |_|/_/\\_\\                 "
};

void
drawSplash()
{
    Logger& log = Logger::getAnonymousLogger();
    for (int i = 0; i < 6; i++) {
        log.info(splash[i]);
    }
}

// typedef void  (*ctor_t)(void);
// extern ctor_t s_ctors[0];
// extern ctor_t e_ctors[0];

void
kmain(BootConfig& bootConfig)
{
    // unsigned ctorCount = ((u64)&e_ctors - (u64)&s_ctors) / sizeof(void*);
    // for (unsigned i = 0; i < ctorCount; i++) {
    //     s_ctors[i]();
    // }

    kern()->setupArch();
    drawSplash();
}