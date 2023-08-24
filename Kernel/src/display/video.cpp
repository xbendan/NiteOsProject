#include <siberix/display/colors.h>
#include <siberix/display/video.h>

Color Color::VgaColors[16] = { Color(0, 0, 0),      Color(0, 0, 170),     Color(0, 170, 0),
                               Color(0, 170, 170),  Color(170, 0, 0),     Color(170, 0, 170),
                               Color(170, 85, 0),   Color(170, 170, 170), Color(85, 85, 85),
                               Color(85, 85, 255),  Color(85, 255, 85),   Color(85, 255, 255),
                               Color(255, 85, 85),  Color(255, 85, 255),  Color(255, 255, 85),
                               Color(255, 255, 255) };

VideoOutput::VideoOutput() {}

VideoOutput::~VideoOutput() {}

PixelVideoOutput::PixelVideoOutput(u8* buffer)
    : m_buffer(buffer) {}

PixelVideoOutput::~PixelVideoOutput() {}