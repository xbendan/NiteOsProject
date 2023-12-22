#include <stdcxx/types.h>

namespace Kern::Hal {
    class InstructionSet
    {
    public:
        virtual void enable()  = 0;
        virtual void disable() = 0;
        bool      isEnabled() { return enabled; }

    private:
        bool enabled;
    };
}