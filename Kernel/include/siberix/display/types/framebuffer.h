#include <siberix/display/video.h>
#include <utils/spinlock.h>

class FramebufferVideoOutput : public PixelVideoOutput {
public:
    FramebufferVideoOutput();
    ~FramebufferVideoOutput();

    void drawRect(Point point, u32 width, u32 height, Color color) override;
    void drawEllipse(Point point, u32 width, u32 height, Color) override;
    void drawText(Point point, const char* text, Color color) override;

    void             setBufferOptions(BufferingOptions b) override;
    BufferingOptions getBufferOptions() override;

    void  setPointAt(Point point, Color color) override;
    Color getPointAt(Point point) override;
    void  update() override;
    u8*   getBuffering() override;
    u8*   getWritableBuffering() override;

private:
    spinlock_t m_lock;
};