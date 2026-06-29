#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class Blackjack : public Game {
private:
    bool running;
    int betAmount;
    int pScore, dScore;
    int state; 
    
    void dealCards();
    void hit();
    void stand();
public:
    void init() override;
    void update() override;
    void draw() override;
    bool isRunning() override;
};