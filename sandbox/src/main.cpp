#include "logger.h"
#include "strategy/compositeLogStrategy.h"
#include "strategy/consoleLogStrategy.h"
#include "strategy/fileLogStrategy.h"

int main()
{
    std::unique_ptr<CompositeLogStrategy> strategy = std::make_unique<CompositeLogStrategy>();
    strategy->addStrategy(std::make_unique<ConsoleLogStrategy>());
    strategy->addStrategy(std::make_unique<FileLogStralegy>("log.txt"));

    Logger::instance().initialize(LogLevel::DEBUG, std::move(strategy));
    LOG_DEBUG("123fjfjfk");
    LOG_INFO("123fjfjfk");
    // Logger logger(LogLevel::DEBUG, std::move(strategy));
    // logger.log(LogLevel::INFO, "QWEQWEQWEQWEWE");
    // logger.log(LogLevel::DEBUG, "VKVJVKKVVJKJVK");
}