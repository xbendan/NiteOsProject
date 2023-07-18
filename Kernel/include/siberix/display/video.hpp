#include <common/typedefs.h>

#include <siberix/device/device.hpp>
#include <siberix/display/colors.hpp>

struct Point {
    int x, y;
};

enum RenderBufferOptions { DirectRender, DoubleBuffering, TripleBuffering };

class Screen {
private:
    u32 m_width;
    u32 m_height;
    u16 m_bytesPerPixel;
};

class VideoCompositionDevice : public Device {
public:
    VideoCompositionDevice();
    ~VideoCompositionDevice();
};