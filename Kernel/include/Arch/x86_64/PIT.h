#include <stdint.h>
#define PIT_FREQUENCY 0x1234DE

namespace PIT
{
    void Initialize(uint32_t frequency);
} // namespace PIT