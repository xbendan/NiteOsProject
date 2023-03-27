#include <Video/TextModePresenter.h>

#include <libkern/objects.h>
#include <stdint.h>

namespace Video
{
    TextModePresenter::TextModePresenter()
      : m_TextBuffer(reinterpret_cast<uint16_t *>(0xb8000)),
        m_OffsetX(0),
        m_OffsetY(0)
    {
        
    }

    void TextModePresenter::Clear()
    {
        for (int y = 0; y < 25; y++)
        {
            for (int x = 0; x < 80; x++)
            {
                m_TextBuffer[(y * 80) + x] = 0x00;
            }
        }
    }

    void TextModePresenter::Text(Point point, const char *text, int fontSize)
    {
        Text(point, text, 15, 0);
    }

    void TextModePresenter::Text(Point point, const char *text, uint8_t foreground, uint8_t background)
    {
        int x = point.x == -1 ? (0) : Integers::ConstraintToRange(0, 80, point.x);
        int y = point.y == -1 ? (m_OffsetY + 1) : Integers::ConstraintToRange(0, 25, point.y);

        if (y >= 25)
        {
            y = 0;
            Clear();
        }

        size_t len = strlen(text);
        for (int i = 0; i < len; i++)
        {
            Text({ x, y }, text[i], foreground, background);
            if (++x >= 80)
            {
                x = 0;
                y++;
                if (y >= 25)
                {
                    y = 0;
                    Clear();
                }
            }
        }

        m_OffsetX = x;
        m_OffsetY = y;
    }

    void TextModePresenter::Text(Point point, char c, uint8_t foreground, uint8_t background)
    {
        m_TextBuffer[(point.y * 80) + point.x] = 
            ((uint16_t) c | (uint16_t) foreground << 8 | (uint16_t) background << 12);
    }

    void TextModePresenter::MoveUp()
    {
        uint16_t *copy = (uint16_t *)(0xb8000 + 160);
        for (int ln = 0; ln < 24; ln++)
        {
            for (int col = 0; col < 80; col++)
            {
                m_TextBuffer[(ln * 80) + col] = copy[((ln + 1) * 80) + col];
            }
        }
    }

    void TextModePresenter::Rectangle(Point point, int width, int height)
    {
        int x = Integers::ConstraintToRange(0, 80, point.x);
        int y = Integers::ConstraintToRange(0, 25, point.y);

        const uint16_t blank = (15 << 8);
        for (int y = point.y; y < point.y + height; y++)
        {
            for (int x = point.x; x < point.x + width; x++)
            {
                m_TextBuffer[(y * 80) + x] = blank;
            }
        }
    }
}