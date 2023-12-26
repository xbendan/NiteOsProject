#include <stdcxx/types.h>

namespace Kern::Sec {
    enum class AccessMode : uint8_t
    {
        Read,
        Write,
        Execute
    };

    enum class AccessResult
    {
        Granted,
        Denied,
        Blanked,
        Randomized
    };

    enum class AccessLevel : uint8_t
    {
        Guest,
        User,
        Administator,
        System,
        Trusted
    };
}