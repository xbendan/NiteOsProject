#include <Timer.h>

class HPETTimer : public Timer
{
private:
    HPET *r_HPET;
    uint64_t clock;
    uint64_t ticks;

public:
    HPETTimer();
    ~HPETTimer();

    inline uint64_t Read(uint64_t reg) { return *((uint64_t *)(r_HPET->Address.Address + reg)); }
    inline void Write(uint64_t reg, uint64_t val) { *((uint64_t *)(r_HPET->Address.Address + reg)) = val; }
    
    virtual void Tick();
    virtual uint64_t CurrentTime(TimeSpan span = Millisecond);
    virtual void Sleep(long milliseconds);
};