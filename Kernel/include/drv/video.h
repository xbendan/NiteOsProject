#include <macros>

namespace Video
{
    void Initialize();

    void ClearScreen();
    void WriteChar(char c);
    void WriteText(const char *str, size_t len);
    void WriteText(const char *str);
} // namespace Video