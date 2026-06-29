#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include "../core/audio.h"

class BlackMarket : public Game {
private:
    bool running;
    int selected;
    bool locked;
    
    const char* items[15] = {
        "1. Hacker Kit ($5k)", "2. VIP Card ($10k)", "3. Tax Evader ($15k)", 
        "4. Bribe Cop ($5k)", "5. Slot Charm ($8k)", "6. Insider ($12k)", 
        "7. Ad Blocker ($2k)", "8. Skeleton Key ($20k)", "9. Vault Map ($7k)", 
        "10. Ghost IP ($6k)", "11. Flash Bot ($15k)", "12. Pump Bot ($15k)", 
        "13. Offshore ($25k)", "14. Diamond Hand ($30k)", "15. THE BOSS ($50k)"
    };
    int prices[15] = {5000, 10000, 15000, 5000, 8000, 12000, 2000, 20000, 7000, 6000, 15000, 15000, 25000, 30000, 50000};

public:
    void init() override {
        running = true; selected = 0;
        // KONTROLA BOHATSTVÍ
        if(Save::balance < 15000 && !Save::perks[14]) locked = true;
        else locked = false;
    }

    void update() override {
        M5Cardputer.update();
        if(M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
        
        if(locked) return; // Pokud nemáš 15k, nic nedělej
        
        if(M5Cardputer.Keyboard.isKeyPressed(';')) { if(selected > 0) selected--; delay(150); }
        if(M5Cardputer.Keyboard.isKeyPressed('.')) { if(selected < 14) selected++; delay(150); }
        
        if(M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
            if(!Save::perks[selected] && Save::balance >= prices[selected]) {
                Save::addBalance(-prices[selected]);
                Save::perks[selected] = true;
                Save::saveAll();
                Audio::playJackpot();
            } else { Audio::playBeep(); }
            delay(200);
        }
    }

    void draw() override {
        auto canvas = UI::getCanvas(); canvas->fillScreen(BLACK); 
        canvas->setTextColor(0xF800, BLACK); // Červený temný vzhled
        
        if(locked) {
            canvas->drawString("BLACK MARKET", 40, 20);
            canvas->setTextColor(WHITE, BLACK);
            canvas->drawString("ACCESS DENIED.", 30, 60);
            canvas->drawString("Come back when you", 20, 80);
            canvas->drawString("have $15,000.", 40, 100);
        } else {
            UI::drawHeader("BLACK MARKET", Save::balance);
            
            int startIdx = selected - 1;
            if (startIdx < 0) startIdx = 0;
            if (startIdx > 11) startIdx = 11;
            
            for(int i=0; i<4; i++) {
                int idx = startIdx + i;
                if(idx < 15) {
                    canvas->setTextColor(idx == selected ? WHITE : 0x7BEF, BLACK);
                    canvas->drawString(items[idx], 20, 35 + (i * 22));
                    if(Save::perks[idx]) canvas->drawString("[OWNED]", 170, 35 + (i * 22));
                }
            }
            UI::drawNeonFrame(10, 35 + ((selected - startIdx) * 22), 220, 20, 0xF800);
        }
        UI::push();
    }
    bool isRunning() override { return running; }
};