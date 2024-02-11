#pragma once

#include <vector>
#include <memory>

#include "logStratery.h"

class CompositeLogStrategy : public LogStrategy
{
private:
    std::vector<std::unique_ptr<LogStrategy>> mStrategies;

public:
    void addStrategy(std::unique_ptr<LogStrategy> strategy);
    void log(const std::string& message) override;
};