#include <stdcxx/types.h>

enum class LoggerLevel : uint8_t
{
    TRACE = 0,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    OFF
};