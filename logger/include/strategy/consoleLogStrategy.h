#pragma once

#include "logStratery.h"

class ConsoleLogStrategy : public LogStrategy
{
public:
    void log(const std::string& message) override;
};