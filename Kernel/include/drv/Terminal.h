class Terminal
{
public:
    void Initialize();

    void ClearScreen();
    void WriteChar(char c);
    void WriteText(const char *str, size_t len);
}