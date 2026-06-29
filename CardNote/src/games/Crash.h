#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class Crash : public Game {
private:
    bool running; int betAmount; float mult; float crashPoint; int state;
public:
    void init() override; void update() override; void draw() override; bool isRunning() override;
};