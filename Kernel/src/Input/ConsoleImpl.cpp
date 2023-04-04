#include <Input/KeyCode.h>
#include <Input/Console.h>

void ConsoleKeyEventHandler(Input::Key key, bool isPressed)
{
    Console *console = g_Console;
    console->AcceptKey(key, isPressed);
}

using namespace Input;

static const char *c_modifiers =    "CapsLock:   |      |       |     |                                              ";
static const char *c_split =        "--------------------------------------------------------------------------------";

Console::Console()
  : m_TextPresenter(new Video::TextModePresenter()),
    m_KeyModifiers(0),
    m_Buffers(new uint8_t[23 * 80]),
    m_OffsetX(1),
    m_OffsetY(0)
{
    Refresh();
}

Console::~Console()
{

}

void Console::Render()
{
    
}

void Console::Refresh()
{
    m_TextPresenter->Clear();
    RenderModifiers();
    m_TextPresenter->Text({ 0, 2 }, '>', 15, 0);
}

void Console::Newline()
{
    m_LineLength = 0;
    m_OffsetX = 1;
    if (m_OffsetY >= 22)
    {
        m_TextPresenter->MoveUp();
        RenderModifiers();
    } else m_OffsetY++;
    m_TextPresenter->Text({ 0, m_OffsetY + 2 }, '>', 15, 0);
}

void Console::RenderModifiers()
{
    bool caps = m_KeyModifiers & KeyModifiersCapsLock;
    bool shift = m_KeyModifiers & KeyModifiersShift;
    bool ctrl = m_KeyModifiers & KeyModifiersCtrl;
    bool alt = m_KeyModifiers & KeyModifiersAlt;

    TextModePresenter *presenter = reinterpret_cast<TextModePresenter *>(m_TextPresenter);

    presenter->Text({ 0, 0 }, c_modifiers, 15, 0);
    presenter->Text({ 9, 0 }, (caps ? " A " : " a "), 0, 15);
    presenter->Text({ 13, 0 }, " Ctrl ", ctrl ? 0 : 15, ctrl ? 15 : 0);
    presenter->Text({ 20, 0 }, " Shift ", shift ? 0 : 15, shift ? 15 : 0);
    presenter->Text({ 28, 0 }, " Alt ", alt ? 0 : 15, alt ? 15 : 0);
    presenter->Text({ 0, 1 }, c_split, 15, 0);
}

void Console::AcceptKey(Input::Key key, bool isPressed)
{
    switch (key)
    {
    case CapsLock:
        if (isPressed)
            if (m_KeyModifiers & KeyModifiersCapsLock)
                m_KeyModifiers &= ~(KeyModifiersCapsLock);
            else
                m_KeyModifiers |= KeyModifiersCapsLock;
        RenderModifiers();
        break;
    case LShift:
    case RShift:
        if (isPressed)
            m_KeyModifiers |= KeyModifiersShift;
        else
            m_KeyModifiers &= ~(KeyModifiersShift);
        RenderModifiers();
        break;
    case LCtrl:
    case RCtrl:
        if (isPressed)
            m_KeyModifiers |= KeyModifiersCtrl;
        else
            m_KeyModifiers &= ~(KeyModifiersCtrl);
        RenderModifiers();
        break;
    case LAlt:
    case RAlt:
        if (isPressed)
            m_KeyModifiers |= KeyModifiersAlt;
        else
            m_KeyModifiers &= ~(KeyModifiersAlt);
        RenderModifiers();
        break;
    case F5:
        m_TextPresenter->Clear();
        RenderModifiers();
        break;
    case Enter:
        if (!isPressed) break;
        Newline();
        break;
    case Space:
        AddChar(' '); break;
    case Backspace:
        
        break;
    default:
        char code = ApplyModifiers(key, m_KeyModifiers);
        if (!code || !isPressed || m_LineLength == 255) break;
        
        AddChar(code);
        break;
    }
}

void Console::AddChar(char c)
{
    m_TextPresenter->Text({ m_OffsetX, 2 + m_OffsetY }, c, 15, 0);
    m_LineLength++;
    if (++m_OffsetX >= 80)
    {
        m_OffsetX = 0;
        if (m_OffsetY >= 22)
        {
            m_TextPresenter->MoveUp();
            RenderModifiers();
        } else m_OffsetY++;
    }
}