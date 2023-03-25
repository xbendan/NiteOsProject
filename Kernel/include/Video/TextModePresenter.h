#include <Video/Video.h>

#include <macros>

namespace Video
{
    class TextModePresenter : VideoPresenter
    {
    private:
        uint16_t *m_TextBuffer;
        uint16_t m_OffsetX;
        uint16_t m_OffsetY;

    public:
        TextModePresenter();
        ~TextModePresenter();

        virtual void Clear();
        virtual void Text(Point point, const char *text, int fontSize);
        virtual void Rectangle(Point point, int width, int height);
    };
} // namespace Video
