namespace System
{
    void Out(const char* fmt, ...);

    void Reboot();
    void Shutdown();
    void Halt();
    void Panic(const char *msg, ...);
} // namespace System
