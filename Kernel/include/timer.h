namespace Timer {
    extern int frequency;
    extern volatile uint64_t ticks;
    extern uint64_t uptimeUs;
    extern long pendingTicks;
    
    void Tick();
    void Sleep(long milliseconds);
}