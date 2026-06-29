#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class VideoPoker : public Game {
private:
    bool running; int betAmount; int state;
    int hand[5]; bool hold[5]; int cursor;
    
    int dealCard();
    void evaluateHand();
public:
    void init() override; void update() override; void draw() override; bool isRunning() override;
};