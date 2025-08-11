#pragma once
#include <SDL.h>

class Game;

class Engine
{
public:
    explicit Engine(Game& game);
    ~Engine();

    bool init(const char* title, int w, int h);
    void run();
    void stop();

    SDL_Renderer* renderer() const { return ren_; }

private:
    Game& game_;
    SDL_Window*  win_ = nullptr;
    SDL_Renderer* ren_ = nullptr;
    bool running_ = false;
};