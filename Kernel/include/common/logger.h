#include <common/printf.h>
#include <common/string.h>

enum LoggerLevel
{
    LOG_INFO,
    LOG_SUCCESS,
    LOG_WARN,
    LOG_ERROR
};

class LoggerReceiver {
    virtual void receive(char c) = 0;
}

class Logger
{
public:
    void log(LoggerLevel level, const char *fmt, va_list args);
    void log(LoggerLevel level, const char *fmt, ...);
    void info(const char *fmt, ...);
    void success(const char *fmt, ...);
    void warn(const char *fmt, ...);
    void error(const char *fmt, ...);
    /// @brief Print current stack trace
    void printStackTrace();
    /// @brief Print current stack trace with message.
    /// @param fmt message format
    /// @param va message arguments
    void printStackTrace(const char *fmt, ...);
    const char *getName();
    Array<String> &getCachedMessages();
    Array<LoggerReceiver &> &getLoggerReceivers();

    /**
     * @brief Get a logger with specific name.
     * 
     * @param name Logger name
     * @return The pointer to new logger.
     */
    static Logger *getLogger(const char *name);
    /**
     * @brief Get an anonymous logger.
     * 
     * @return The pointer to new logger.
     */
    static Logger *getAnonymousLogger();

private:
    const char *name;
    SizedArrayList<String, 256> messages;
    char *record;
}