#include "strategy/compositeLogStrategy.h"

void CompositeLogStrategy::addStrategy(std::unique_ptr<LogStrategy> strategy)
{
    mStrategies.push_back(std::move(strategy));
}

void CompositeLogStrategy::log(const std::string &message)
{
    for (auto &&strategy : mStrategies)
    {
        strategy->log(message);
    }
    
}