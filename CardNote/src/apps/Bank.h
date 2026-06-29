#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class Bank : public Game {
private:
    bool running;
    int state; // 0=Banka, 1=Hacking, 2=Vyhra, 3=Prohra
    
    const char* dict[12] = {"MATRIX", "HACKER", "PHANTM", "SYSTEM", "BREACH", "ROUTER", "ACCESS", "CRYPTO", "TROJAN", "BYPASS", "LOCKER", "SIGNAL"};
    const char* activeWords[6]; 
    int targetIdx;
    int selectedWord;
    int attempts;
    int lastMatch;

    int getMatches(const char* a, const char* b) {
        int m = 0;
        for(int i=0; i<6; i++) { if(a[i] == b[i]) m++; } 
        return m;
    }

    void startHeist() {
        state = 1; attempts = 3; // Pouze 3 pokusy!
        selectedWord = 0; lastMatch = -1;
        int picked[6];
        for(int i=0; i<6; i++) {
            bool ok = false;
            while(!ok) {
                picked[i] = random(0, 12);
                ok = true;
                for(int j=0; j<i; j++) { if(picked[j] == picked[i]) ok = false; }
            }
            activeWords[i] = dict[picked[i]];
        }
        targetIdx = random(0, 6); 
    }

public:
    void init() override { running = true; state = 0; }
    
    void update() override {
        M5Cardputer.update();
        if(state == 0) { 
            if(M5Cardputer.Keyboard.isKeyPressed('l')) { Save::takeLoan(1000); Audio::playBeep(); delay(200); } 
            if(M5Cardputer.Keyboard.isKeyPressed('p') && Save::debt > 0) { Save::payLoan(1000); Audio::playBeep(); delay(200); } 
            if(M5Cardputer.Keyboard.isKeyPressed('h')) { startHeist(); Audio::playBeep(); delay(200); } 
            if(M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
        }
        else if(state == 1) { 
            if(M5Cardputer.Keyboard.isKeyPressed(';')) { if(selectedWord>0) selectedWord--; delay(150); } 
            if(M5Cardputer.Keyboard.isKeyPressed('.')) { if(selectedWord<5) selectedWord++; delay(150); } 
            
            if(M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
                if(selectedWord == targetIdx) {
                    Save::addBalance(5000); 
                    Audio::playWin(); 
                    state = 2; 
                } else {
                    attempts--;
                    lastMatch = getMatches(activeWords[selectedWord], activeWords[targetIdx]);
                    Audio::playBeep();
                    if(attempts <= 0) { 
                        Save::addBalance(-(Save::balance / 2)); 
                        UI::shake();
                        state = 3; 
                    }
                }
                delay(200);
            }
        } 
        else { 
            if(M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) state = 0;
        }
    }
    
    void draw() override {
        auto canvas = UI::getCanvas(); 
        canvas->fillScreen(UI::C_BG); 
        UI::drawHeader("MAFIA BANK", Save::balance);
        
        if(state == 0) {
            canvas->setTextColor(UI::C_TEXT, UI::C_BG); canvas->drawString("LOANS (10% Interest)", 10, 35);
            canvas->setTextColor(0xF800, UI::C_BG); canvas->drawString((String("DEBT: $") + Save::debt).c_str(), 10, 55); 
            
            canvas->setTextColor(UI::C_PRIM, UI::C_BG); 
            canvas->drawString("L: Take $1000 | P: Pay $1000", 5, 85);
            
            canvas->setTextColor(UI::C_ACCENT, UI::C_BG); 
            canvas->drawString("H: HACK VAULT (Risk: 50% Bal)", 5, 115);
        }
        else if(state == 1) {
            canvas->setTextColor(UI::C_SEC, UI::C_BG); 
            canvas->drawString((String("ATTEMPTS LEFT: ") + attempts).c_str(), 5, 30);
            
            for(int i=0; i<6; i++) {
                canvas->setTextColor(i == selectedWord ? UI::C_TEXT : UI::C_PRIM, UI::C_BG);
                canvas->drawString(activeWords[i], 20, 50 + (i * 11)); 
            }
            
            if(lastMatch >= 0) {
                canvas->setTextColor(0xF800, UI::C_BG); 
                canvas->drawString((String("Match: ") + lastMatch + "/6").c_str(), 130, 70);
            }
        }
        else if(state == 2) {
            canvas->setTextColor(0x07E0, UI::C_BG); 
            canvas->drawString("VAULT UNLOCKED!", 30, 50);
            canvas->drawString("+ $5000", 70, 70);
        }
        else if(state == 3) {
            canvas->setTextColor(0xF800, UI::C_BG); 
            canvas->drawString("SECURITY TRIPPED!", 20, 50);
            canvas->drawString("HALF BALANCE LOST!", 20, 70);
        }
        
        UI::push();
    }
    
    bool isRunning() override { return running; }
};