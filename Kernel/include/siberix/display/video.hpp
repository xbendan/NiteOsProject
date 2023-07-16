#include <common/typedefs.h>

#include <siberix/device/device.hpp>

struct Point {
    u32 x, y;
};

struct Color {
    u8 r, g, b, a;
};

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