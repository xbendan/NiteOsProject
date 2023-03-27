#include <Input/KeyCode.h>
#include <Video/TextModePresenter.h>

void ConsoleKeyEventHandler(Input::Key key, bool isPressed);

class Console
{
private:
    Video::TextModePresenter *m_TextPresenter;
    uint8_t m_KeyModifiers;
    uint8_t *m_Buffers;
    bool m_Init;
    uint8_t m_OffsetX, m_OffsetY;
    uint8_t m_LineLength;

public:
    Console();
    ~Console();

    bool IsInitialized() { return m_Init; }
    void Render();
    void RenderModifiers();
    void Refresh();
    void AcceptKey(Input::Key key, bool isPressed);
    void AddChar(char c);
};

extern Console *g_Console;