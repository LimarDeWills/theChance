#include "engine.hpp"
#include "game.hpp"
#include <iostream>
#include <cstdio>

Engine::Engine(Game &game) : game_(game) {}
Engine::~Engine()
{
    if (ren_)
        SDL_DestroyRenderer(ren_);
    if (win_)
        SDL_DestroyWindow(win_);
    SDL_Quit();
}

bool Engine::init(const char *title, int w, int h)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
        return false;
    }
    win_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    if (!win_)
    {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << "\n";
        return false;
    }
    ren_ = SDL_CreateRenderer(win_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren_)
    {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << "\n";
        return false;
    }
    running_ = true;
    return true;
}

void Engine::stop() { running_ = false; }

void Engine::run()
{
    const double dt = 1.0 / 60.0; // 60 updates per second
    const double max_acc = 0.25;  // defend against "spiral of death"

    const double freq = static_cast<double>(SDL_GetPerformanceFrequency());
    uint64_t prev = SDL_GetPerformanceCounter();
    uint64_t fps_timer = prev;
    double acc = 0.0;
    int frames = 0, updates = 0;

    while (running_)
    {
        // --- Events ---
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running_ = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                running_ = false;
            game_.onEvent(e);
        }

        // --- Time ---
        uint64_t now = SDL_GetPerformanceCounter();
        double frame_time = (now - prev) / freq;
        prev = now;
        if (frame_time > max_acc)
            frame_time = max_acc;
        acc += frame_time;

        // --- Fixed updates ---
        while (acc >= dt)
        {
            game_.update(dt);
            acc -= dt;
            updates++;
        }

        // --- Render ---
        SDL_SetRenderDrawColor(ren_, 20, 20, 20, 255);
        SDL_RenderClear(ren_);
        game_.render(ren_);
        SDL_RenderPresent(ren_);
        frames++;

        // --- Simple FPS/UPS title ---
        if ((now - fps_timer) / freq >= 1.0)
        {
            char title[128];
            std::snprintf(title, sizeof(title), "theChance | %d fps, %d ups", frames, updates);
            SDL_SetWindowTitle(win_, title);
            frames = 0;
            updates = 0;
            fps_timer = now;
        }
    }
}