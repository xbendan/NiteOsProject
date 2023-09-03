#include <common/logger.h>
#include <siberix/display/video.h>

class VgaTextOutput : public VideoOutput {
public:
    VgaTextOutput();
    ~VgaTextOutput();

    void drawRect(Point point, u32 width, u32 height, Color color) override;
    void drawEllipse(Point point, u32 width, u32 height, Color) override;
    void drawText(Point point, const char* text, Color color) override;
    void drawTextCode(Point point, char code, Color color) override;

    inline void setBufferOptions(bool isDoubleBuffering) override {}
    inline bool getBufferOptions() override { return false; }

    void newline();

private:
    u16* m_buffer;
    u32  m_x, m_y;
};

class VgaTextReceiver : public LoggerReceiver {
public:
    void receive(char c) override {
        m_vga->drawTextCode({ -1, -1 }, c, Color(VgaTextColor::White));
    }
    void receive(const char* str) override {
        m_vga->drawText({ -1, -1 }, str, Color(VgaTextColor::White));
    }

private:
    VgaTextOutput* m_vga;
};