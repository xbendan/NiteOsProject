#include <common/typedefs.h>
#include <siberix/device/types.h>
#include <siberix/display/colors.h>

struct Point {
    int x, y;
};

enum class VideoProjectionOptions {
    PrimaryOnly,
    Duplicated,
    Extended
};

class ScreenDevice : public VisualOutputDevice {
private:
    u32 m_width;
    u32 m_height;
    u16 m_bytesPerPixel;
};

class VideoOutput {
public:
    virtual void drawRect(Point point, u32 width, u32 height, Color color) = 0;
    virtual void drawEllipse(Point point, u32 width, u32 height, Color)    = 0;
    virtual void drawText(Point point, const char* text, Color color)      = 0;

    virtual void setBufferOptions(bool isDoubleBuffering);
    virtual bool getBufferOptions();

protected:
    bool m_isDoubleBuffering;
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

class VideoCompositionDevice : public Device {
public:
    VideoCompositionDevice();
    ~VideoCompositionDevice();

private:
    VideoProjectionOptions    m_projectionMode;
    LinkedList<ScreenDevice*> m_screens;
    LinkedList<VideoOutput&>  m_videoOutputs;
};