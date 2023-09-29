#include <common/format.h>
#include <common/logger.h>

#include <utils/array.h>

Logger Logger::anonymousLogger("_");

char*              logPrefix   = "[%s]";
static const char* logLevels[] = { "Info", "Success", "Warn", "Error" };

void
Logger::initialize()
{
    static utils::Array<Logger*, 256> _loggers = utils::Array<Logger*, 256>();
    static utils::Array<LoggerReceiver*, 60> _receivers =
      utils::Array<LoggerReceiver*, 60>();

    loggers   = static_cast<utils::Collection<Logger*>*>(&_loggers);
    receivers = static_cast<utils::Collection<LoggerReceiver*>*>(&_receivers);
}

void
Logger::log(LoggerLevel level, const char* fmt, va_list args)
{
    char buf0[8];
    char buf1[256];
    strfmt(buf0, "[%s] ", logLevels[level]);
    strfmt(buf1, fmt, args);
    for (int i = 0; i < receivers->size(); i++) {
        LoggerReceiver* r = *((*receivers)[i].get());
        r->write(buf0);
        r->write(buf1);
    };

    receivers->forEach(utils::function::Function<void(LoggerReceiver*)>(
      [](LoggerReceiver* r) -> void { r->write(buf0); }));
}

void
Logger::log(LoggerLevel level, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(level, fmt, args);
    va_end(args);
}

void
Logger::info(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(LOG_INFO, fmt, args);
    va_end(args);
}

void
Logger::success(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(LOG_SUCCESS, fmt, args);
    va_end(args);
}

void
Logger::warn(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(LOG_WARN, fmt, args);
    va_end(args);
}

void
Logger::error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(LOG_ERROR, fmt, args);
    va_end(args);

    log(LOG_ERROR, "!! System is shutting to avoid further damage !!\n");

    for (;;)
        asm("cli; hlt");
}

void
Logger::error(void* regs, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(LOG_ERROR, fmt, args);
    va_end(args);

    log(LOG_ERROR, "!! System is shutting to avoid further damage !!\n");
    printStackTrace(regs);
}

const char*
Logger::getName()
{
    return m_name;
}

utils::Collection<Logger*>*
Logger::getLoggers()
{
    return loggers;
}

utils::Collection<LoggerReceiver*>*
Logger::getLoggerReceivers()
{
    return receivers;
}

Logger&
Logger::getLogger(const char* name)
{
    for (int i = 0; i < loggers->size(); i++) {
        Logger* loggerPtr = *(loggers->get(i).get());
        if (strcmp(loggerPtr->getName(), name) == 0) {
            return *loggerPtr;
        }
    }

    Logger* logger = new Logger(name);
    Logger::getLoggers()->add(logger);
    return *logger;
}

Logger&
Logger::getAnonymousLogger()
{
    return anonymousLogger;
}