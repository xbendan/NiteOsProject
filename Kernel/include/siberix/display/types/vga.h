#include <common/logger.h>
#include <siberix/display/video.h>

class VgaTextOutput;

class VgaTextReceiver : public LoggerReceiver {
public:
    VgaTextReceiver() = default;
    VgaTextReceiver(VgaTextOutput* vga)
        : m_vga(vga) {}
    ~VgaTextReceiver() {}

    void receive(char c) override;
    void receive(const char* str) override;
    void setOutput(VgaTextOutput* vga);

private:
    VgaTextOutput* m_vga;
};

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

    VgaTextReceiver* getReceiver() { return &m_receiver; }

    void newline();

private:
    VgaTextReceiver m_receiver;
    u16*            m_buffer;
    u32             m_x, m_y;
};