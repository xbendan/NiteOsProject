#include <siberix/display/types/framebuffer.hpp>

FramebufferVideoOutput::FramebufferVideoOutput()
    : m_bytesPerPixel(4) {}

FramebufferVideoOutput::~FramebufferVideoOutput() {}

void FramebufferVideoOutput::drawRect(Point point,
                                      u32   width,
                                      u32   height,
                                      Color color) {
    u8* p;
    u8* buffer = getWritableBuffering();
    int _x, _y;
    for (int x = 0; x < width; x++) {
        _x = point.x + x;
        if (_x < 0 || _x > m_width) {
            continue;
        }
        for (int y = 0; y < height; y++) {
            _y = point.y + y;
            if (_y < 0 || _y > m_height) {
                continue;
            }
            p = &(buffer[((_y * m_width) + _x) * m_bytesPerPixel]);

            p[0] = color.r;
            p[1] = color.g;
            p[2] = color.b;
        }
    }
}

void FramebufferVideoOutput::drawEllipse(Point point,
                                         u32   width,
                                         u32   height,
                                         Color) {}

void FramebufferVideoOutput::drawText(Point       point,
                                      const char* text,
                                      Color       color) {}

void FramebufferVideoOutput::setBufferOptions(BufferingOptions b) {}

BufferingOptions FramebufferVideoOutput::getBufferOptions() {}

void FramebufferVideoOutput::setPointAt(Point point, Color color) {
    if (point.x > m_width || point.y > m_height) {
        return;
    }
    u64 bytesOffset = m_bytesPerPixel * (point.x + (point.y * m_width));
    u8* p           = &(getWritableBuffering()[bytesOffset]);

    p[0] = color.r;
    p[1] = color.g;
    p[2] = color.b;
}

Color FramebufferVideoOutput::getPointAt(Point point, Color color) {}

u32* FramebufferVideoOutput::getBuffering() { return m_primaryBuffering; }

u32* FramebufferVideoOutput::getWritableBuffering() {
    switch (m_bufferOptions) {
        case BufferingOptions::DirectRender: {
            return m_primaryBuffering;
        }
        case BufferingOptions::DoubleBuffering:
        case BufferingOptions::TripleBuffering: {
            return m_doubleBuffering;
        }
    }
}