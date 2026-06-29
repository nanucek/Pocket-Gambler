#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class Mines : public Game {
private:
    bool running; 
    int state; 
    int betAmount; 
    float multiplier;
    int grid[5][5]; // 0=Empty, 1=Bomb, 2=Cleared Empty
    int cx, cy; // Cursor positions
public:
    void init() override; 
    void update() override; 
    void draw() override; 
    bool isRunning() override;
};