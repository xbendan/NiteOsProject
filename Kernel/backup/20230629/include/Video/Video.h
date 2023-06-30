#pragma once

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
        VideoPresenter();
        ~VideoPresenter();

        virtual void Clear() = 0;
        virtual void Text(Point point, const char *text, int fontSize) = 0;
        virtual void Rectangle(Point point, int width, int height) = 0;
    };

    class VideoController
    {
    private:
        VideoPresenter *m_VideoMode;
    
    public:
        VideoController();
        ~VideoController();
        VideoPresenter *Presenter();
    };
}