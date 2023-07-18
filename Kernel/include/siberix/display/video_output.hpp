#include <siberix/display/video.hpp>

class VideoOutput {
public:
    virtual void drawRect(Point point, u32 width, u32 height, Color color) = 0;
    virtual void drawEllipse(Point point, u32 width, u32 height, Color)    = 0;
    virtual void drawText(Point point, const char* text, Color color)      = 0;

    virtual bool        isTextOnly();
    RenderBufferOptions getBufferOptions();

    template <typename T>
    T getBuffer();

protected:
    void* m_unifiedBuffer;
};