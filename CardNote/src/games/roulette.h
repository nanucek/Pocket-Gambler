#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class Roulette : public Game {
private:
    bool running;
    bool spinning;
    int betAmount;
    int selectedColor; 
    int resultNumber;
    
    void spinRoulette();
    void checkWin();
public:
    void init() override;
    void update() override;
    void draw() override;
    bool isRunning() override;
};