#include <Input/KeyCode.h>
#include <Video/TextModePresenter.h>

void ConsoleKeyEventHandler(Input::Key key, bool isPressed);

using namespace Video;

class Console
{
private:
    TextModePresenter *m_TextPresenter;
    uint8_t m_KeyModifiers;
    uint8_t *m_Buffers;
    uint8_t m_OffsetX, m_OffsetY;
    uint8_t m_LineLength;

public:
    Console();
    ~Console();

    void Render();
    void RenderModifiers();
    void Refresh();
    void Newline();
    void AcceptKey(Input::Key key, bool isPressed);
    void AddChar(char c);
};

extern Console *g_Console;