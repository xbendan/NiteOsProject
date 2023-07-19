#include <siberix/display/video_output.hpp>

class FramebufferVideoOutput : public PixelVideoOutput {
public:
    FramebufferVideoOutput();
    ~FramebufferVideoOutput();

    void drawRect(Point point, u32 width, u32 height, Color color) override;
    void drawEllipse(Point point, u32 width, u32 height, Color) override;
    void drawText(Point point, const char* text, Color color) override;

    BufferingOptions getBufferOptions();

private:
    u32* m_buffer;
}