#pragma once

#include <common/printf.h>
#include <common/string.h>

enum LoggerLevel {
    LOG_INFO,
    LOG_SUCCESS,
    LOG_WARN,
    LOG_ERROR
};

class LoggerReceiver {
    virtual void receive(char c)          = 0;
    virtual void receive(const char* str) = 0;
};

class Logger {
public:
    /// @brief
    /// @param level
    /// @param fmt
    /// @param args
    void        log(LoggerLevel level, const char* fmt, va_list args);
    /// @brief
    /// @param level
    /// @param fmt
    /// @param
    void        log(LoggerLevel level, const char* fmt, ...);
    /// @brief
    /// @param fmt
    /// @param
    void        info(const char* fmt, ...);
    /// @brief
    /// @param fmt
    /// @param
    void        success(const char* fmt, ...);
    /// @brief
    /// @param fmt
    /// @param
    void        warn(const char* fmt, ...);
    /// @brief
    /// @param fmt
    /// @param
    void        error(const char* fmt, ...);
    /// @brief Print current stack trace
    void        printStackTrace();
    /// @brief Print current stack trace with message.
    /// @param fmt message format
    /// @param va message arguments
    void        printStackTrace(const char* fmt, ...);
    /// @brief
    /// @return
    const char* getName();
    /// @brief
    /// @return
    // Array<String>& getCachedMessages();
    /// @brief
    /// @return
    LinkedList<LoggerReceiver&>& getLoggerReceivers();

    /**
     * @brief Get a logger with specific name.
     *
     * @param name Logger name
     * @return The pointer to new logger.
     */
    static Logger&                               getLogger(const char* name);
    /**
     * @brief Get an anonymous logger.
     *
     * @return The pointer to new logger.
     */
    static Logger&                               getAnonymousLogger();
    static SizedArrayList<LoggerReceiver&, 256>& getReceivers();

private:
    const char*                 name;
    SizedArrayList<String, 256> messages;

    static SizedArrayList<LoggerReceiver&, 256> receivers;
    static Logger                               anonymousLogger;
    static Logger                               defaultLogger;
};