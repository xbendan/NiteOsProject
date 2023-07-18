#include <siberix/display/video.hpp>

class VideoOutput {
public:
    virtual void drawRect(Point point, u32 width, u32 height, Color color) = 0;
    virtual void drawEllipse(Point point, u32 width, u32 height, Color)    = 0;
    virtual void drawText(Point point, const char* text, Color color)      = 0;

    virtual bool     isTextOnly();
    BufferingOptions getBufferOptions();

protected:
};

class PixelVideoOutput : public VideoOutput {
public:
    virtual void  setPointAt(Point point, Color color) = 0;
    virtual Color getPointAt(Point point)              = 0;
    virtual void  update();
    virtual u32*  getBuffering();
    virtual u32*  getWritableBuffering();

private:
    u32* m_primaryBuffering;
    u32* m_doubleBuffering;
    u8   m_bytesPerPixel;
};