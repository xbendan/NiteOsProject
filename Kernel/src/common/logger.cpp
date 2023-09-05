#include <arch/x86_64/serial.h>
#include <common/logger.h>
#include <common/printf.h>
#include <siberix/display/types/vga.h>

SizedArrayList<Logger*, 256>        Logger::loggers   = SizedArrayList<Logger*, 256>();
SizedArrayList<LoggerReceiver*, 60> Logger::receivers = SizedArrayList<LoggerReceiver*, 60>();
Logger                              Logger::anonymousLogger("system");

extern SerialPortLoggerReceiver _serialPortReceiver;

void Logger::log2all(char* text) {
    for (int i = 0; i < receivers.length(); i++) {
        // asm volatile("cli; hlt");
        // _serialPortReceiver.receive(text);
        receivers[0]->receive(text);
    }
}

void Logger::log(LoggerLevel level, const char* fmt, va_list args) {
    int   d = 0;
    u64   u = 0;
    char* s = nullptr;
    char  c = ' ';
    char  buf[64];

    while (*fmt) {
        char ch = *fmt;
        if (ch == '%') {
            switch (*++fmt) {
                case 'b':
                case 'B':
                    d = va_arg(args, int);
                    log2all(itoa(d, buf, 2));
                    break;

                case 'x':
                case 'X':
                    u = va_arg(args, u64);
                    log2all(utoa(u, buf, 16));
                    break;

                case 'i':
                    d = va_arg(args, int);
                    log2all(itoa(d, buf, 10));
                    break;

                case 'u':
                    u = va_arg(args, u64);
                    log2all(utoa(u, buf, 10));
                    break;

                case '%':
                    log2all("%");
                    // videoOutput->drawTextCode(
                    //     Point{ -1, -1 },
                    //     '%',
                    //     Color::VgaColors[static_cast<u8>(VgaTextColor::Black)]);
                    break;

                case 'c':
                    log2all(va_arg(args, char*));
                    // c = va_arg(args, int);
                    // videoOutput->drawTextCode(
                    //     Point{ -1, -1 }, c,
                    //     Color::VgaColors[static_cast<u8>(VgaTextColor::Black)]);
                    break;

                case 's':
                    s = va_arg(args, char*);
                    // log2all(s ? s : "(NULL)");
                    break;

                default:
                    break;
            }
        } else {
            log2all(&ch);
            // videoOutput->drawTextCode(Point{ -1, -1 }, ch, Color(VgaTextColor::Black));
        }
        fmt++;
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