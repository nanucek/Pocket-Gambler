#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class Plinko : public Game {
private:
    bool running; int betAmount; int state;
    int ballRow; int ballCol; // Simulace pozice
    float multipliers[9] = {10.0, 3.0, 1.5, 0.5, 0.1, 0.5, 1.5, 3.0, 10.0};
public:
    void init() override; void update() override; void draw() override; bool isRunning() override;
};
