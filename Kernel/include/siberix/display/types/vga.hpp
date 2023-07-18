#include <siberix/display/video_output.hpp>

class VgaTextOutput : public VideoOutput {
public:
    VgaTextOutput();
    ~VgaTextOutput();

    void drawRect(u32 x, u32 y, u32 width, u32 height, Color color) override;
    void drawEllipse(u32 x, u32 y, u32 width, u32 height, Color) override;
    void drawText(
        u32 x, u32 y, const char* text, u16 fsize, Color color) override;

    bool                isTextOnly() { return true; }
    RenderBufferOptions getBufferOptions() {
        return RenderBufferOptions::DirectRender;
    }

    void newline();

private:
    u16* m_buffer;
    u32  m_x, m_y;
};