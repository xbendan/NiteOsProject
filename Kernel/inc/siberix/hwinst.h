#include <stdcxx/types.h>

namespace Kern::Hal {
    class InstructionSet
    {
    public:
        virtual void enable()  = 0;
        virtual void disable() = 0;
        Boolean      isEnabled() { return enabled; }

    private:
        Boolean enabled;
    };
}