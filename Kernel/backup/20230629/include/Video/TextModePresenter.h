#pragma once

#include <Video/Video.h>

#include <macros>

namespace Video
{
    class TextModePresenter : public VideoPresenter
    {
    private:
        uint16_t *m_TextBuffer;
        uint16_t m_OffsetX;
        uint16_t m_OffsetY;

    public:
        TextModePresenter();
        ~TextModePresenter();

        void Clear() override;
        void Text(Point point, const char *text, int fontSize) override;
        void Text(Point point, const char *text, uint8_t foreground, uint8_t background);
        void Text(Point point, char c, uint8_t foreground, uint8_t background);
        void MoveUp();
        void Rectangle(Point point, int width, int height) override;
    };
} // namespace Video
