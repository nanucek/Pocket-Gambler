#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class Dice : public Game {
private:
    bool running; int betAmount; int choice; // 0: <7, 1: 7, 2: >7
    int d1, d2; int state;
public:
    void init() override; void update() override; void draw() override; bool isRunning() override;
};