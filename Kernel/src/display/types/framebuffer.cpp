#include <common/string.h>
#include <siberix/core/runtimes.h>
#include <utils/alignment.h>

#include <siberix/display/types/framebuffer.hpp>

FramebufferVideoOutput::FramebufferVideoOutput()
    : m_bytesPerPixel(4) {
    BootConfig& boot = exec()->getBootConfig();

    if (!boot.graphic.address) {
        return;
    }

    m_buffer        = reinterpret_cast<u8*>(boot.graphic.address);
    m_bytesPerPixel = boot.graphic.bpp;
    m_width         = boot.graphic.width;
    m_height        = boot.graphic.height;

    setBufferOptions(BufferingOptions::DirectRender);
}

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

void FramebufferVideoOutput::setBufferOptions(BufferingOptions b) {
    if (b == m_bufferOptions) {
        return;
    }
    switch (b) {
        case BufferingOptions::DirectRender: {
            if (m_doubleBuffering != nullptr) {
                u64 size = m_width * m_height * m_bytesPerPixel;
                exec()->getMemory().free4KPages(m_buffer,
                                                alignUp(size, PAGE_SIZE_4K));
            }
            break;
        }
        case BufferingOptions::DoubleBuffering: {
            if (!m_doubleBuffering) {
                u64 size          = m_width * m_height * m_bytesPerPixel;
                m_doubleBuffering = exec()->getMemory().alloc4KPages(
                    alignUp(size, PAGE_SIZE_4K));
            }
        }
    }
    m_bufferOptions = b;
}

BufferingOptions FramebufferVideoOutput::getBufferOptions() {
    return m_bufferOptions;
}

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

Color FramebufferVideoOutput::getPointAt(Point point) {}

void FramebufferVideoOutput::update() {
    if (m_bufferOptions != BufferingOptions::DoubleBuffering) {
        return;
    }
    acquireLockIntDisable(&m_lock);
    memcpy(m_buffer, m_doubleBuffering, m_width * m_height * m_bytesPerPixel);
    releaseLock(&m_lock);
}

u8* FramebufferVideoOutput::getBuffering() { return m_buffer; }

u8* FramebufferVideoOutput::getWritableBuffering() {
    switch (m_bufferOptions) {
        case BufferingOptions::DirectRender: {
            return m_buffer;
        }
        case BufferingOptions::DoubleBuffering:
        case BufferingOptions::TripleBuffering: {
            return m_doubleBuffering;
        }
    }
}