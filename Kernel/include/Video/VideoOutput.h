#include <Video/Video.h>
#include <macros>

namespace Video
{
    class VideoOutput
    {

    };

    class BufferOutput
    {
    protected:
        uint32_t *m_Buffer;
    };

    class MonitorOutput : VideoOutput
    {
    protected:
        int m_Width, m_Height;
        void *m_Buffer;

    public:
        void *Buffer() { return m_Buffer; }
        int GetWidth() { return m_Width; }
        int GetHeight() { return m_Height; }
    };
} // namespace Video
