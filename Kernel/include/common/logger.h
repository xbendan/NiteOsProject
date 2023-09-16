#pragma once

#include <common/format.h>
#include <common/string.h>
#include <utils/array.h>
#include <utils/linked_list.h>

enum LoggerLevel
{
    LOG_INFO,
    LOG_SUCCESS,
    LOG_WARN,
    LOG_ERROR
};

class LoggerReceiver
{
public:
    virtual void write(char ch) {}
    virtual void write(const char* str) {}
};

class Logger
{
public:
    Logger(const char* _name)
      : name(_name)
    {
    }
    Logger() {}
    ~Logger() {}

    /// @brief
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
    /// @brief
    /// @param fmt
    /// @param
    void        error(void* regs, const char* fmt, ...);
    /// @brief Print current stack trace
    void        printStackTrace(void* regs);
    /// @brief
    /// @return
    const char* getName();

    static void                                 log2all(char* text);
    /// @brief
    /// @return
    static SizedArrayList<LoggerReceiver*, 60>& getLoggerReceivers();
    static SizedArrayList<Logger*, 256>&        getLoggers();
    /**
     * @brief Get a logger with specific name.
     *
     * @param name Logger name
     * @return The pointer to new logger.
     */
    static Logger&                              getLogger(const char* name);
    /**
     * @brief Get an anonymous logger.
     *
     * @return The pointer to new logger.
     */
    static Logger&                              getAnonymousLogger();

public:
    const char* name;
    // SizedArrayList<String, 256> messages;

    static SizedArrayList<LoggerReceiver*, 60> receivers;
    static SizedArrayList<Logger*, 256>        loggers;
    static Logger                              anonymousLogger;
};