#pragma once

#include <iostream>

enum class LogLevel;

class LogStrategy
{
public:
    virtual ~LogStrategy() = default;
    virtual void log(const std::string& message) = 0;
};