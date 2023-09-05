#include <arch/x86_64/kaddr.h>
#include <common/string.h>
#include <siberix/display/types/vga.h>

void VgaTextReceiver::receive(char c) {
    // m_vga->drawTextCode({ -1, -1 }, c, Color(VgaTextColor::White));
}

void VgaTextReceiver::receive(const char* str) {
    // _vga.drawText({ -1, -1 }, str, Color(VgaTextColor::White));
}

VgaTextOutput::VgaTextOutput()
    : m_receiver(VgaTextReceiver(this)),
      m_buffer(reinterpret_cast<u16*>(IOVB(0xb8000))) {}

VgaTextOutput::~VgaTextOutput() {}

void VgaTextOutput::drawRect(Point point, u32 width, u32 height, Color color) {
    char c  = ' ';
    u8   bg = Color::getVgaColorIndex(color, VgaTextColor::White);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int _x = point.x + x;
            int _y = point.y + y;
            if (_x < 0 || _y < 0) {
                continue;
            }
            m_buffer[(_y * 80) + _x] = (bg << 12) | (bg << 8) | c;
        }
    }
}

void VgaTextOutput::drawEllipse(Point point, u32 width, u32 height, Color color) {}

void VgaTextOutput::drawText(Point point, const char* text, Color color) {
    if (point.x < 0 || point.y < 0) {
        point.x = m_x;
        point.y = m_y;
    }
    int len = strlen(text);
    int n   = 0;
    while (len--) {
        if (point.x + 1 >= 80) {
            newline();
            point.x = 0;
            point.y = m_y;
        }
        char c  = text[n++];
        u16* p  = &(m_buffer[(point.y * 80) + point.x]);
        u8   fg = Color::getVgaColorIndex(color, VgaTextColor::White);
        u8   bg = (*p) >> 12;

        *p = (bg << 12) | (fg << 8) | c;

        point.x++;
    }
    m_x = point.x;
    m_y = point.y;
}

void VgaTextOutput::drawTextCode(Point point, char code, Color color) {
    if (point.x < 0 || point.y < 0) {
        point.x = m_x;
        point.y = m_y;
    }
    if (++point.x >= 80) {
        newline();
        point.x = 0;
        point.y = m_y;
    }
    u16* p  = &(m_buffer[(point.y * 80) + point.x]);
    u8   fg = Color::getVgaColorIndex(color, VgaTextColor::White);
    u8   bg = (*p) >> 12;

    *p = (bg << 12) | (fg << 8) | code;
}

void VgaTextOutput::newline() {
    m_x = 0;
    if (++m_y >= 25) {
        for (int i = 0; i < 24; i++) {
            memcpy(&(m_buffer[i * 80]), &(m_buffer[(i + 1) * 80]), 80);
        }
        m_y--;
    }
}