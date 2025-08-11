#include "game.hpp"
#include <cmath>
#include <algorithm>
#include <random>

static inline int ifloor(double v) { return (int)std::floor(v); }

Game::Game()
{
    // Build a bigger-than-window level so scrolling is visible
    buildLevel(64, 48); // 64x48 tiles => 2048x1536 px
    // Start player somewhere inside
    x_ = 5 * tileSize_ + 6;
    y_ = 5 * tileSize_ + 6;
    updateCamera();
}

void Game::buildLevel(int w, int h)
{
    // Generate a simple bordered room with some inner blocks/corridors.
    mapW_ = w;
    mapH_ = h;
    level_.assign(h, std::string(w, '.'));

    // Border walls
    for (int x = 0; x < w; ++x)
    {
        level_[0][x] = '#';
        level_[h - 1][x] = '#';
    }
    for (int y = 0; y < h; ++y)
    {
        level_[y][0] = '#';
        level_[y][w - 1] = '#';
    }

    // Simple maze-ish strips
    for (int y = 4; y < h - 4; y += 6)
    {
        for (int x = 2; x < w - 2; ++x)
        {
            if ((x % 9) != 0)
                level_[y][x] = '#';
        }
    }
    // Vertical pillars
    for (int x = 6; x < w - 6; x += 12)
    {
        for (int y = 3; y < h - 3; ++y)
        {
            if (y % 7 != 0)
                level_[y][x] = '#';
        }
    }
}

bool Game::isWall(int tx, int ty) const
{
    if (ty < 0 || ty >= mapH_ || tx < 0 || tx >= mapW_)
        return true; // out of bounds is solid
    return level_[ty][tx] == '#';
}

void Game::onEvent(const SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
    {
        const bool down = (e.type == SDL_KEYDOWN);
        if (e.key.repeat)
            return; // ignore OS key repeat

        // Use scancodes so movement works regardless of keyboard layout
        switch (e.key.keysym.scancode)
        {
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
            in_.up = down;
            break;

        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
            in_.down = down;
            break;

        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
            in_.left = down;
            break;

        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
            in_.right = down;
            break;

        default:
            break;
        }
    }
}

void Game::moveX(double dx)
{
    if (dx == 0.0)
        return;
    double nx = x_ + dx;

    if (dx > 0.0)
    {
        // Moving right: check the right edge of the player AABB
        int right = ifloor((nx + size_ - 1) / tileSize_);
        int top = ifloor(y_ / tileSize_);
        int bottom = ifloor((y_ + size_ - 1) / tileSize_);
        if (isWall(right, top) || isWall(right, bottom))
        {
            nx = right * tileSize_ - size_; // snap left of the wall
        }
    }
    else
    {
        // Moving left: check the left edge
        int left = ifloor(nx / tileSize_);
        int top = ifloor(y_ / tileSize_);
        int bottom = ifloor((y_ + size_ - 1) / tileSize_);
        if (isWall(left, top) || isWall(left, bottom))
        {
            nx = (left + 1) * tileSize_; // snap right of the wall
        }
    }

    // Clamp to world bounds (in pixels)
    nx = std::clamp(nx, 0.0, (double)worldW() - size_);
    x_ = nx;
}

void Game::moveY(double dy)
{
    if (dy == 0.0)
        return;
    double ny = y_ + dy;

    if (dy > 0.0)
    {
        // Moving down: check bottom edge
        int bottom = ifloor((ny + size_ - 1) / tileSize_);
        int left = ifloor(x_ / tileSize_);
        int right = ifloor((x_ + size_ - 1) / tileSize_);
        if (isWall(left, bottom) || isWall(right, bottom))
        {
            ny = bottom * tileSize_ - size_;
        }
    }
    else
    {
        // Moving up: check top edge
        int top = ifloor(ny / tileSize_);
        int left = ifloor(x_ / tileSize_);
        int right = ifloor((x_ + size_ - 1) / tileSize_);
        if (isWall(left, top) || isWall(right, top))
        {
            ny = (top + 1) * tileSize_;
        }
    }

    ny = std::clamp(ny, 0.0, (double)worldH() - size_);
    y_ = ny;
}

void Game::updateCamera()
{
    // Center camera on player
    const double targetX = x_ + size_ * 0.5 - viewW_ * 0.5;
    const double targetY = y_ + size_ * 0.5 - viewH_ * 0.5;

    // Clamp camera so it doesn't show space outside the world
    camX_ = std::clamp(targetX, 0.0, std::max(0.0, (double)worldW() - viewW_));
    camY_ = std::clamp(targetY, 0.0, std::max(0.0, (double)worldH() - viewH_));
}

void Game::update(double dt)
{
    // Direction from input
    double vx = (in_.right ? 1.0 : 0.0) - (in_.left ? 1.0 : 0.0);
    double vy = (in_.down ? 1.0 : 0.0) - (in_.up ? 1.0 : 0.0);

    // Normalize to keep diagonal speed equal
    const double len = std::sqrt(vx * vx + vy * vy);
    if (len > 0.0)
    {
        vx /= len;
        vy /= len;
    }

    // Axis-separated movement with wall collisions
    moveX(vx * speed_ * dt);
    moveY(vy * speed_ * dt);

    // Update camera after we moved
    updateCamera();
}

void Game::render(SDL_Renderer *r)
{
    // Clear is done by Engine; we draw the world relative to camera.

    // Compute visible tile range (slightly extended to avoid gaps)
    const int firstTX = std::max(0, ifloor(camX_ / tileSize_) - 1);
    const int firstTY = std::max(0, ifloor(camY_ / tileSize_) - 1);
    const int lastTX = std::min(mapW_ - 1, ifloor((camX_ + viewW_) / tileSize_) + 1);
    const int lastTY = std::min(mapH_ - 1, ifloor((camY_ + viewH_) / tileSize_) + 1);

    // Draw tiles
    for (int ty = firstTY; ty <= lastTY; ++ty)
    {
        for (int tx = firstTX; tx <= lastTX; ++tx)
        {
            if (level_[ty][tx] == '#')
            {
                SDL_Rect cell{
                    tx * tileSize_ - (int)camX_,
                    ty * tileSize_ - (int)camY_,
                    tileSize_, tileSize_};
                SDL_SetRenderDrawColor(r, 70, 70, 80, 255); // wall
                SDL_RenderFillRect(r, &cell);
            }
        }
    }

    // Draw player
    SDL_Rect rect{
        (int)(x_ - camX_),
        (int)(y_ - camY_),
        size_, size_};
    SDL_SetRenderDrawColor(r, 220, 220, 220, 255);
    SDL_RenderFillRect(r, &rect);
}