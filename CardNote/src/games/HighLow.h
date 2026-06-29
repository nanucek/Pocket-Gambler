#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class HighLow : public Game {
private:
    bool running; int betAmount; int currentNum; int nextNum; int state;
public:
    void init() override; void update() override; void draw() override; bool isRunning() override;
};