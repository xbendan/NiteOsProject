#include <stdcxx/linked-list.h>
#include <stdcxx/string.h>
#include <xtra-io/writer.h>
#include <xtra-logging/level.h>

struct Log
{
    uint64_t          _timestamp;
    LoggerLevel       _level;
    Std::String<Utf8> _message;

    static void info(Std::String<Utf8> format, ...);
    static void success(Std::String<Utf8> format, ...);
    static void warn(Std::String<Utf8> format, ...);
    static void error(Std::String<Utf8> format, ...);
    static void fatal(Std::String<Utf8> format, ...);
};

class Logger
{
public:
    Logger(bool useTimestamps = true, bool useCaches = true)
      : m_useTimestamps(useTimestamps)
      , m_useCaches(useCaches)
    {
    }

    void log(LoggerLevel level, Std::String<Utf16> format, ...);
    void log(LoggerLevel level, Std::String<Utf8> message);
    void printStackTrace(void* registers);

private:
    Std::LinkedList<Io::TextWriter*> m_writers;

    bool m_useTimestamps;
    bool m_useCaches;
};