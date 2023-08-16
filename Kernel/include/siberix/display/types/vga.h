#include <siberix/display/video.h>

class VgaTextOutput : public VideoOutput {
public:
    VgaTextOutput();
    ~VgaTextOutput();

    void drawRect(Point point, u32 width, u32 height, Color color) override;
    void drawEllipse(Point point, u32 width, u32 height, Color) override;
    void drawText(Point point, const char* text, Color color) override;

    inline bool getBufferOptions() override { return false; }

    void newline();

private:
    u16* m_buffer;
    u32  m_x, m_y;
};