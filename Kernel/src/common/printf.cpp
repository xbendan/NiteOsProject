#include <common/printf.h>
#include <common/typedefs.h>
#include <siberix/display/video.h>

VideoOutput* videoOutput;

void puts(const char* msg) {
    videoOutput->drawText(
        Point{ -1, -1 }, msg, Color::VgaColors[static_cast<u8>(VgaTextColor::Black)]);
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void vprintf(const char* fmt, va_list args) {}