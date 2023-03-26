#include <Utils/LinkedList.h>

#include <macros>

namespace Video
{
    struct Point { int x, y; };
    struct Color { uint8_t r, g, b, a; };
    class VideoOutput;

    class VideoPresenter
    {
    protected:
        LinkedList<VideoOutput *> m_Outputs;

    public:
        virtual void Clear() = 0;
        virtual void Text(Point point, const char *text, int fontSize) = 0;
        virtual void Rectangle(Point point, int width, int height) = 0;
    };
}