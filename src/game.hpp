#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include <cassert>

class Game {
public:
    Game(); // builds a big level
    void onEvent(const SDL_Event& e);
    void update(double dt);
    void render(SDL_Renderer* r);

private:
    // --- Input state ---
    struct Input { bool up=false, down=false, left=false, right=false; } in_;

    // --- Tile map (grid) ---
    const int tileSize_ = 32;             // pixels per tile
    std::vector<std::string> level_;      // rows of '#' (wall) and '.' (empty)
    int mapW_ = 0, mapH_ = 0;             // in tiles
    bool isWall(int tx, int ty) const;    // solid query
    void buildLevel(int w, int h);        // generate a simple large map

    // --- Player (in world pixels) ---
    double x_ = 64.0;
    double y_ = 64.0;
    int    size_  = 20;                   // player AABB size in pixels
    double speed_ = 160.0;                // px/sec

    // Move along axes with AABB collisions against walls
    void moveX(double dx);
    void moveY(double dy);

    // --- Viewport / Camera ---
    // For now viewport equals the window size (engine uses 800x600).
    // If you later make the window resizable, pass actual size here.
    int viewW_ = 800;
    int viewH_ = 600;

    // Camera top-left in world pixels
    double camX_ = 0.0;
    double camY_ = 0.0;

    // World size in pixels
    inline int worldW() const { return mapW_ * tileSize_; }
    inline int worldH() const { return mapH_ * tileSize_; }

    // Follow player and clamp camera to world bounds
    void updateCamera();
};
