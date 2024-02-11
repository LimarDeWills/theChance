#include "strategy/fileLogStrategy.h"

FileLogStralegy::FileLogStralegy(const std::string &logFilePath)
{
    logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) 
        std::cerr << "Failed to open log file: " << logFilePath << std::endl;
}

void FileLogStralegy::log(const std::string &message)
{
    if (!logFile.is_open()) {
        std::cerr << "Log file is not open." << std::endl;
        return;
    }
    
    logFile << message << std::endl;
}
