#include <siberix/display/video.hpp>

class VideoOutput {
public:
    virtual void drawRect(u32 x, u32 y, u32 width, u32 height, Color color) = 0;
    virtual void drawEllipse(u32 x, u32 y)                                  = 0;
};