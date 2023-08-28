#include <common/string.h>
#include <siberix/core/runtimes.h>
#include <siberix/display/types/framebuffer.h>
#include <siberix/init/boot.h>
#include <siberix/mm/memory.h>
#include <utils/alignment.h>

FramebufferVideoOutput::FramebufferVideoOutput()
    : PixelVideoOutput(nullptr) {
    BootConfig& boot = siberix()->getBootConfig();

    if (!boot.graphic[0].address) {
        return;
    }

    m_buffer        = reinterpret_cast<u8*>(boot.graphic[0].address);
    m_bytesPerPixel = boot.graphic[0].bpp;
    m_width         = boot.graphic[0].width;
    m_height        = boot.graphic[0].height;

    setBufferOptions(false);
}

FramebufferVideoOutput::~FramebufferVideoOutput() {}

void FramebufferVideoOutput::drawRect(Point point, u32 width, u32 height, Color color) {
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

void FramebufferVideoOutput::drawEllipse(Point point, u32 width, u32 height, Color) {}

void FramebufferVideoOutput::drawText(Point point, const char* text, Color color) {}

void FramebufferVideoOutput::drawTextCode(Point point, char code, Color color) {}

void FramebufferVideoOutput::setBufferOptions(bool isDoubleBuffering) {
    if (m_isDoubleBuffering == isDoubleBuffering) {
        return;
    }
    u64 size = m_width * m_height * m_bytesPerPixel;
    if (isDoubleBuffering) {
        siberix()->getMemory().free4KPages((u64)m_buffer,
                                           alignUp(size, static_cast<u64>(PAGE_SIZE_4K)));
    } else {
        m_doubleBuffering = reinterpret_cast<u8*>(
            siberix()->getMemory().alloc4KPages(alignUp(size, static_cast<u64>(PAGE_SIZE_4K))));
    }
    m_isDoubleBuffering = isDoubleBuffering;
}

bool FramebufferVideoOutput::getBufferOptions() { return m_isDoubleBuffering; }

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

Color FramebufferVideoOutput::getPointAt(Point point) {
    if (point.x > m_width || point.y > m_height) {
        return Color(0, 0, 0);
    }
    u64 bytesOffset = m_bytesPerPixel * (point.x + (point.y * m_width));
    u8* p           = &(getBuffering()[bytesOffset]);

    return Color(p[0], p[1], p[2]);
}

void FramebufferVideoOutput::update() {
    if (!m_isDoubleBuffering) {
        return;
    }
    acquireLockIntDisable(&m_lock);
    memcpy(m_buffer, m_doubleBuffering, m_width * m_height * m_bytesPerPixel);
    releaseLock(&m_lock);
}

u8* FramebufferVideoOutput::getBuffering() { return m_buffer; }

u8* FramebufferVideoOutput::getWritableBuffering() {
    return m_isDoubleBuffering ? m_doubleBuffering : m_buffer;
}