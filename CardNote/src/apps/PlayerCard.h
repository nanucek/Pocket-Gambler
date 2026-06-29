#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"

class PlayerCard : public Game {
private:
    bool running;
public:
    void init() override { running = true; }
    
    void update() override {
        M5Cardputer.update();
        if(M5Cardputer.Keyboard.isKeyPressed('t')) { 
            UI::setTheme((UI::currentTheme + 1) % 3); delay(200); 
        }
        if(M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
    }
    
    void draw() override {
        auto canvas = UI::getCanvas(); 
        canvas->fillScreen(UI::C_BG); 
        UI::drawHeader("PLAYER STATS", Save::balance);
        
        canvas->setTextColor(UI::C_PRIM, UI::C_BG);
        canvas->drawString((String("LVL: ") + Save::level + " | XP: " + Save::xp).c_str(), 5, 35);
        canvas->drawString((String("HI-SCORE: $") + Save::highScore).c_str(), 120, 35);
        
        int minVal = Save::history[0];
        int maxVal = Save::history[0];
        for(int i=0; i<50; i++) {
            if(Save::history[i] < minVal) minVal = Save::history[i];
            if(Save::history[i] > maxVal) maxVal = Save::history[i];
        }
        if(maxVal == minVal) maxVal = minVal + 1; 
        
        int gX = 10; int gY = 60; int gW = 220; int gH = 45;
        UI::drawNeonFrame(gX-2, gY-2, gW+4, gH+4, UI::C_SEC); 
        
        for(int i=1; i<50; i++) {
            int x1 = gX + ((i-1) * gW) / 49;
            int y1 = gY + gH - ((Save::history[i-1] - minVal) * gH) / (maxVal - minVal);
            int x2 = gX + (i * gW) / 49;
            int y2 = gY + gH - ((Save::history[i] - minVal) * gH) / (maxVal - minVal);
            canvas->drawLine(x1, y1, x2, y2, UI::C_ACCENT);
        }
        
        canvas->setTextColor(UI::C_SEC, UI::C_BG); 
        canvas->drawString("Press 't' to Change Theme", 5, 115);
        
        UI::push();
    }
    
    bool isRunning() override { return running; }
};