#include "Mines.h"

void Mines::init() { 
    running = true; 
    state = 0; 
    betAmount = 10; 
    multiplier = 1.0f; 
    cx = 2; 
    cy = 2; 
}

void Mines::update() {
    M5Cardputer.update();
    
    if(state == 0) { // Sázky
        if(M5Cardputer.Keyboard.isKeyPressed(',')) { if(betAmount > 10) betAmount -= 10; delay(150); }
        if(M5Cardputer.Keyboard.isKeyPressed('/')) { if(betAmount + 10 <= Save::balance) betAmount += 10; delay(150); }
        if(M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) && Save::balance >= betAmount) {
            Save::addBalance(-betAmount);
            
            // Vygenerování 3 bomb do mřížky 5x5
            for(int r = 0; r < 5; r++) {
                for(int c = 0; c < 5; c++) {
                    grid[r][c] = 0;
                }
            }
            int b = 0; 
            while(b < 3) { 
                int rr = random(0, 5);
                int cc = random(0, 5); 
                if(grid[rr][cc] == 0) { 
                    grid[rr][cc] = 1; 
                    b++; 
                } 
            }
            
            state = 1; 
            multiplier = 1.0f; 
            Audio::playBeep(); 
            delay(200);
        }
    } 
    else if(state == 1) { // Samotné hraní
        if(M5Cardputer.Keyboard.isKeyPressed(';')) { if(cy > 0) cy--; delay(150); } // UP
        if(M5Cardputer.Keyboard.isKeyPressed('.')) { if(cy < 4) cy++; delay(150); } // DOWN
        if(M5Cardputer.Keyboard.isKeyPressed(',')) { if(cx > 0) cx--; delay(150); } // LEFT
        if(M5Cardputer.Keyboard.isKeyPressed('/')) { if(cx < 4) cx++; delay(150); } // RIGHT
        
        // Cashout tlačítko (C)
        if(M5Cardputer.Keyboard.isKeyPressed('C')) { 
            Save::addBalance(betAmount * multiplier); 
            state = 3; 
            Audio::playWin(); 
            delay(200);
        }
        
        // Odhalení políčka
        if(M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) { 
            if(grid[cy][cx] == 0) { 
                grid[cy][cx] = 2; 
                multiplier += 0.2f; 
                Audio::playBeep(); 
            }
            else if(grid[cy][cx] == 1) { 
                state = 2; 
                UI::shake(); // Prohra!
            } 
            delay(200);
        }
    } 
    else { 
        // Stavy 2 (Prohra) a 3 (Výhra/Cashout)
        if(M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) state = 0; 
    }
    
    // Návrat do menu
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
}

void Mines::draw() {
    auto canvas = UI::getCanvas(); 
    canvas->fillScreen(UI::C_BG); 
    UI::drawHeader("NEON MINES", Save::balance);
    
    if(state == 0) { 
        canvas->setTextColor(GREEN, UI::C_BG); 
        canvas->drawString((String("Bet: $") + betAmount).c_str(), 10, 60); 
    }
    
    // Vykreslování mřížky herní plochy
    for(int r = 0; r < 5; r++) {
        for(int c = 0; c < 5; c++) {
            int px = 80 + (c * 18); 
            int py = 30 + (r * 18);
            uint16_t color = UI::C_SEC;
            
            // Zvýraznění kurzoru
            if(state == 1 && cx == c && cy == r) color = UI::C_TEXT; 
            
            if(grid[r][c] == 2) {
                // Bezpečně odhaleno
                canvas->fillRect(px, py, 14, 14, UI::C_PRIM); 
            }
            else if(state > 1 && grid[r][c] == 1) {
                // Ukázání bomb po skončení hry
                canvas->fillRect(px, py, 14, 14, RED); 
            }
            else {
                // Skryté pole
                canvas->drawRect(px, py, 14, 14, color); 
            }
        }
    }
    
    // Informační texty na levé straně obrazovky
    if(state == 1) {
        canvas->setTextColor(UI::C_TEXT, UI::C_BG); 
        canvas->drawString((String("x") + multiplier).c_str(), 10, 50);
        canvas->setTextColor(UI::C_PRIM, UI::C_BG); 
        canvas->drawString("C: Cashout", 5, 115);
    } 
    else if(state == 2) { 
        canvas->setTextColor(RED, UI::C_BG); 
        canvas->drawString("BOOM!", 10, 50); 
    } 
    else if(state == 3) { 
        canvas->setTextColor(GREEN, UI::C_BG); 
        canvas->drawString("CASHED!", 10, 50); 
    }
    
    UI::push();
}

bool Mines::isRunning() { return running; }