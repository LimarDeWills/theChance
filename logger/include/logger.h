/**
 * @file Logger.h
 * @brief Singleton logger class that utilizes logging strategies.
 *
 * Provides logging functionality across the application using a singleton pattern.
 * Allows logging with different strategies and levels. The logger must be initialized
 * with a log level and a logging strategy before use.
 */

#pragma once

#include <iostream>
#include <memory>

#include "strategy/logStratery.h"

/**
 * @enum LogLevel
 * @brief Log levels for controlling logging output.
 */
enum class LogLevel
{
    INFO,    ///< An informational message.
    WARNING, ///< A warning message.
    ERROR,   ///< An error message.
    FATAL,   ///< A fatal error message, indicating the program may crash.
    DEBUG    ///< A debug-level message.
};

/**
 * @class Logger
 * @brief Singleton logger class.
 *
 * Logger provides a global logging facility, using different logging strategies
 * and severity levels. It must be initialized with a log level and a strategy
 * before it can be used.
 */
class Logger
{
private:
    LogLevel mLogLevel; ///< Current log level of the logger.
    std::unique_ptr<LogStrategy> mLogStrategy; ///< The logging strategy used by the logger.
    bool mInitialized = false; ///< Indicates whether the logger has been initialized.

    /**
     * @brief Private constructor for the singleton pattern.
     */
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

public:
    /**
     * @brief Returns the singleton instance of Logger.
     * @return Reference to the singleton Logger instance.
     */
    static Logger& instance();

    /**
     * @brief Initializes the logger with a log level and a logging strategy.
     * @param level The log level for the logger.
     * @param strategy The logging strategy to use.
     *
     * This method must be called before the logger can be used. If the logger
     * is already initialized, it will output an error message.
     */
    void initialize(LogLevel level, std::unique_ptr<LogStrategy> strategy);

    void log(const LogLevel level, const std::string &message);
    void setLogLevel(const LogLevel level);
    void setLogStrategy(std::unique_ptr<LogStrategy> strategy);
    std::string getCurrentTimestamp();
    std::string formatMessage(const LogLevel level, const std::string &message);
    std::string levelToString(const LogLevel level);
};

#define LOG_DEBUG(msg) Logger::instance().log(LogLevel::DEBUG, msg)
#define LOG_INFO(msg) Logger::instance().log(LogLevel::INFO, msg)
#define LOG_WARNING(msg) Logger::instance().log(LogLevel::WARNING, msg)
#define LOG_ERROR(msg) Logger::instance().log(LogLevel::ERROR, msg)
#define LOG_FATAL(msg) Logger::instance().log(LogLevel::FATAL, msg)