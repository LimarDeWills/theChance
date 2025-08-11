#include "engine.hpp"
#include "game.hpp"

int main() {
    Game game;
    Engine engine(game);
    if (!engine.init("theChance", 800, 600))
        return 1;
    engine.run();
    return 0;
}