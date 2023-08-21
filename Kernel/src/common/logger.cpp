#include <common/logger.h>
#include <common/printf.h>

Logger Logger::anonymousLogger("system");

void Logger::log(LoggerLevel level, const char* fmt, va_list args) {}

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

Logger& Logger::getLogger(const char* name) {
    ListNode<Logger&>* node = Logger::getLoggers().first();
    while (node != nullptr) {
        if (strcmp(node->obj.getName(), name) == 0) {
            return node->obj;
        }
        node = node->next;
    }

    Logger* logger = new Logger(name);
    Logger::getLoggers().add(*logger);
    return *logger;
}

Logger& Logger::getAnonymousLogger() { return anonymousLogger; }