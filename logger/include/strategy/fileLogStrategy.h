#pragma once

#include <fstream>
#include "logStratery.h"

class FileLogStralegy : public LogStrategy
{
private:
    std::ofstream logFile;
public:
    FileLogStralegy(const std::string& logFilePath);
    void log(const std::string& message) override;
};