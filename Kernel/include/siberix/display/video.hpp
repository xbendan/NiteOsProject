#include <common/typedefs.h>

#include <siberix/device/types.hpp>
#include <siberix/display/colors.hpp>

struct Point {
    int x, y;
};

enum class BufferingOptions {
    DirectRender,
    DoubleBuffering,
    TripleBuffering
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

class VideoCompositionDevice : public Device {
public:
    VideoCompositionDevice();
    ~VideoCompositionDevice();

private:
    VideoProjectionOptions    m_projectionMode;
    LinkedList<ScreenDevice*> m_screens;
    LinkedList<VideoOutput&>  m_videoOutputs;
};