#include <common/logger.h>
#include <siberix/init/boot.h>

#include <siberix/core/runtimes.hpp>

static const char*[] splash = [
    " _____  _  _                  _       ",
    "/  ___|(_)| |                (_)      ",
    "\\ `--.  _ | |__    ___  _ __  _ __  __",
    " `--. \\| || '_ \\  / _ \\| '__|| |\\ \\/ /",
    "/\\__/ /| || |_) ||  __/| |   | | >  < ",
    "\\____/ |_||_.__/  \\___||_|   |_|/_/\\_\\"
];

void drawSplash() {
    Logger& log = Logger::getAnonymousLogger();
    for (int i = 0; i < 6; i++) {
        log.info(splash[i]);
    }
}

void kmain(BootConfig& bootConfig) {
    runtime()->setup();
    drawSplash();
}