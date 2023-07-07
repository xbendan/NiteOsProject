#include <siberix/core/runtimes.hpp>
#include <siberix/init/boot.h>

static const char[] splash = [
    " _____  _  _                  _       ",
    "/  ___|(_)| |                (_)      ",
    "\\ `--.  _ | |__    ___  _ __  _ __  __",
    " `--. \\| || '_ \\  / _ \\| '__|| |\\ \\/ /",
    "/\\__/ /| || |_) ||  __/| |   | | >  < ",
    "\\____/ |_||_.__/  \\___||_|   |_|/_/\\_\\"
];

void main(BootConfig& bootConfig) { runtime()->setup(); }