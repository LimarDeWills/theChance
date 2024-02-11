#include <chrono>
#include <iomanip>

#include "logger.h"

Logger::Logger() {};

Logger& Logger::instance() 
{
    static Logger instance;
    return instance;
}

void Logger::initialize(LogLevel level, std::unique_ptr<LogStrategy> strategy) 
{
    if (!mInitialized) 
    {
        setLogLevel(level);
        setLogStrategy(std::move(strategy));
        mInitialized = true;
    } 
    else 
    {
        std::cerr << "Logger is already initialized." << std::endl;
    }
}

void Logger::log(const LogLevel level, const std::string &message)
{
    if (mLogLevel >= level)
        mLogStrategy->log(formatMessage(level, message));
}

void Logger::setLogLevel(const LogLevel level)
{
    mLogLevel = level;
}

void Logger::setLogStrategy(std::unique_ptr<LogStrategy> strategy)
{
    mLogStrategy = std::move(strategy);
}

std::string Logger::getCurrentTimestamp() 
{
    auto now = std::chrono::system_clock::now();
    
    auto now_c = std::chrono::system_clock::to_time_t(now);
    
    std::tm now_tm = *std::localtime(&now_c);
    
    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    
    return oss.str();
}

std::string Logger::formatMessage(const LogLevel level, const std::string &message)
{
    const std::string time = getCurrentTimestamp();
    std::string result = "[" + time + "] " + levelToString(level) + ": " + message;
    return result;
}

std::string Logger::levelToString(const LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::FATAL:
        return "FATAL";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    default:
        return "UNKNOWN";
    }
}
