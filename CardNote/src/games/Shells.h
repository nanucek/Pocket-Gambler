#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class Shells : public Game {
private:
    bool running; int betAmount; int selected; int ballPos; int state;
public:
    void init() override; void update() override; void draw() override; bool isRunning() override;
};