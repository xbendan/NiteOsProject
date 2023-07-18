#include <siberix/display/video_output.hpp>

class FramebufferVideoOutput : public VideoOutput {
private:
    u32* m_buffer;
}