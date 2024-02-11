#include "strategy/consoleLogStrategy.h"

void ConsoleLogStrategy::log(const std::string &message)
{
    std::cout << message << std::endl;
}