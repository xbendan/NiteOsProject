#include <arch/x86_64/serial.h>
#include <common/format.h>
#include <common/logger.h>
#include <siberix/display/types/vga.h>

SizedArrayList<Logger*, 256>        Logger::loggers   = SizedArrayList<Logger*, 256>();
SizedArrayList<LoggerReceiver*, 60> Logger::receivers = SizedArrayList<LoggerReceiver*, 60>();
Logger                              Logger::anonymousLogger("system");
static char                         buf[256];

void Logger::log(LoggerLevel level, const char* fmt, va_list args) {
    memset(buf, 0, 256);
    strfmts(buf, fmt, args);
    for (int i = 0; i < receivers.length(); i++) {
        receivers[i]->receive(buf);
    }
}

void Logger::log(LoggerLevel level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(level, fmt, args);
    va_end(args);
}

void Logger::info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(LOG_INFO, fmt, args);
    va_end(args);
}

void Logger::success(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(LOG_SUCCESS, fmt, args);
    va_end(args);
}

void Logger::warn(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(LOG_WARN, fmt, args);
    va_end(args);
}

void Logger::error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log(LOG_ERROR, fmt, args);
    va_end(args);

    log(LOG_ERROR, "System is shutting to avoid further damage.");
    printStackTrace();
}

void Logger::printStackTrace() {}

void Logger::printStackTrace(const char* fmt, ...) {}

const char* Logger::getName() { return name; }

SizedArrayList<Logger*, 256>& Logger::getLoggers() { return loggers; }

SizedArrayList<LoggerReceiver*, 60>& Logger::getLoggerReceivers() { return receivers; }

Logger& Logger::getLogger(const char* name) {
    for (int i = 0; i < loggers.length(); i++) {
        if (strcmp(loggers[i]->getName(), name) == 0) {
            return *loggers[i];
        }
    }

    Logger* logger = new Logger(name);
    Logger::getLoggers().add(logger);
    return *logger;
}

Logger& Logger::getAnonymousLogger() { return anonymousLogger; }