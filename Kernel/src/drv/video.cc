#include <drv/video.h>
#include <macros>
#include <string.h>
#include <stdarg.h>

namespace Video
{
    uint16_t *buffers;
    
    uint16_t horizontalOffset;
    uint16_t verticalOffset;

    void Initialize() {
        buffers = (uint16_t *) 0xb8000;
        horizontalOffset = 0;
        verticalOffset = 0;
    }

    void Newline()
    {
        verticalOffset++;
        horizontalOffset = 0;

        if (verticalOffset >= 25)
        {
            verticalOffset = 0;
            ClearScreen();
        }
    }

    bool CheckIfLineFulled()
    {
        if (horizontalOffset >= 80)
        {
            Newline();
            return true;
        }
        else return false;
    }

    void ClearScreen()
    {
        for (size_t h = 0; h < 25; h++) {
            for (size_t v = 0; v < 80; v++) {
                buffers[(h * 80) + v] = 0x0;
            }
        }
    }

    void WriteChar(char c) {
        buffers[(verticalOffset * 80) + horizontalOffset] = ((uint16_t)c | (uint16_t)15 << 8);
        horizontalOffset++;
        CheckIfLineFulled();
    }

    void WriteText(const char *str) {
        WriteText(str, strlen(str));
    }

    void WriteText(const char *str, size_t len) {
        for (size_t ptr = 0; ptr < len; ptr++)
            WriteChar(str[ptr]);
        Newline();
    }
} // namespace Video
