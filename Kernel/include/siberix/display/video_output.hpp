#include <siberix/display/video.hpp>

class VideoOutput {
public:
    virtual void drawRect(Point point, u32 width, u32 height, Color color) = 0;
    virtual void drawEllipse(Point point, u32 width, u32 height, Color)    = 0;
    virtual void drawText(Point point, const char* text, Color color)      = 0;

    virtual void             setBufferOptions(BufferingOptions b);
    virtual BufferingOptions getBufferOptions();

protected:
    BufferingOptions m_bufferOptions;
};

class PixelVideoOutput : public VideoOutput {
public:
    virtual void  setPointAt(Point point, Color color) = 0;
    virtual Color getPointAt(Point point)              = 0;
    virtual void  update();
    virtual u8*   getBuffering();
    virtual u8*   getWritableBuffering();

protected:
    u8* m_buffer;
    u8* m_doubleBuffering;
    u8  m_bytesPerPixel;
    u32 m_width;
    u32 m_height;
};