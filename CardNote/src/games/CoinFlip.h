#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class CoinFlip : public Game {
private:
    bool running; int betAmount; int choice; // 0=Heads, 1=Tails
    int result; int state; // 0=Betting, 1=Flipping, 2=Result
public:
    void init() override; void update() override; void draw() override; bool isRunning() override;
};