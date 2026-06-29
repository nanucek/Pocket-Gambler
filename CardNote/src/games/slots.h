#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class Slots : public Game {
private:
    bool running;
    int state; // 0=Wait, 1=Spinning, 2=Result
    char symbols[6] = {'X', 'O', '#', '$', '%', '7'};
    int r1, r2, r3;
    int spinTimer;
    String msg;

public:
    void init() override { running = true; state = 0; r1=0; r2=1; r3=2; msg="Press ENTER to Spin ($50)"; }

    void update() override {
        M5Cardputer.update();
        if(M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
        
        if(state == 0 || state == 2) {
            if(M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) && Save::balance >= 50) {
                Save::addBalance(-50);
                state = 1; spinTimer = 20; // 20 framů točení
                Audio::playBeep();
            }
        } 
        else if(state == 1) {
            r1 = random(0, 6); r2 = random(0, 6); r3 = random(0, 6);
            spinTimer--;
            if(spinTimer <= 0) {
                state = 2;
                if(r1 == r2 && r2 == r3) {
                    if(symbols[r1] == '7') { Save::addBalance(5000); msg = "JACKPOT! +$5000"; Audio::playJackpot(); UI::confetti();}
                    else { Save::addBalance(500); msg = "WINNER! +$500"; Audio::playWin(); }
                } 
                else if(r1 == r2 || r2 == r3 || r1 == r3) {
                    Save::addBalance(100); msg = "SMALL WIN +$100"; Audio::playBeep();
                } 
                else { msg = "YOU LOSE."; }
            }
            delay(50); // Rychlost točení
        }
    }

    void draw() override {
        auto canvas = UI::getCanvas(); canvas->fillScreen(UI::C_BG); 
        UI::drawHeader("SLOT MACHINE", Save::balance);
        
        canvas->setTextColor(UI::C_TEXT, UI::C_BG);
        canvas->drawString(msg.c_str(), 10, 35);
        
        // Vykreslení válců
        UI::drawNeonFrame(30, 60, 50, 50, UI::C_SEC);
        UI::drawNeonFrame(95, 60, 50, 50, UI::C_SEC);
        UI::drawNeonFrame(160, 60, 50, 50, UI::C_SEC);
        
        canvas->setTextSize(4);
        canvas->setTextColor(UI::C_ACCENT, UI::C_BG);
        canvas->drawString(String(symbols[r1]).c_str(), 42, 70);
        canvas->drawString(String(symbols[r2]).c_str(), 107, 70);
        canvas->drawString(String(symbols[r3]).c_str(), 172, 70);
        canvas->setTextSize(2);
        
        UI::push();
    }
    bool isRunning() override { return running; }
};