#include <Video/Video.h>
namespace Video
{
    class FramebufferPresenter : VideoPresenter
    {
        virtual void Clear();
        virtual void Text(Point point, const char *text, int fontSize);
        virtual void Rectangle(Point point, int width, int height);
    };
} // namespace Video
