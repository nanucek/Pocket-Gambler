#include "Roulette.h"

void Roulette::init() {
    running = true; spinning = false;
    betAmount = 10; selectedColor = 0; resultNumber = -1;
}

void Roulette::spinRoulette() {
    spinning = true;
    Save::addBalance(-betAmount);
    
    for(int i=0; i<15; i++) {
        resultNumber = random(0, 37); 
        draw();
        Audio::playBeep();
        delay(10 + (i * 5)); 
    }
    
    spinning = false;
    checkWin();
}

void Roulette::checkWin() {
    int winColor = (resultNumber == 0) ? 2 : (resultNumber % 2 == 0 ? 0 : 1); 
    
    if (winColor == selectedColor) {
        if (selectedColor == 2) {
            Save::addBalance(betAmount * 14); 
            Audio::playJackpot();
        } else {
            Save::addBalance(betAmount * 2); 
            Audio::playWin();
        }
    }
}

void Roulette::update() {
    M5Cardputer.update();
    
    if (!spinning) {
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { if(betAmount > 10) betAmount -= 10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { if(betAmount + 10 <= Save::balance) betAmount += 10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(';')) { selectedColor = (selectedColor + 1) % 3; delay(150); }
        
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) && Save::balance >= betAmount) {
            spinRoulette();
        }
    }
    
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
}

void Roulette::draw() {
    auto canvas = UI::getCanvas();
    canvas->fillScreen(BLACK);
    UI::drawHeader("CYBER ROULETTE", Save::balance);
    
    const char* colors[] = {"RED (x2)", "BLACK (x2)", "GREEN (x14)"};
    
    char betStr[64];
    snprintf(betStr, sizeof(betStr), "Bet: $%d on %s", betAmount, colors[selectedColor]);
    canvas->setTextColor(WHITE, BLACK);
    canvas->drawString(betStr, 10, 40);
    
    if (resultNumber >= 0) {
        char resStr[32];
        snprintf(resStr, sizeof(resStr), "Result: %d", resultNumber);
        
        uint16_t resCol = NEON_GREEN;
        if (resultNumber != 0) resCol = (resultNumber % 2 == 0) ? RED : DARKGREY;
        
        UI::drawNeonFrame(80, 70, 80, 30, resCol);
        canvas->setTextColor(WHITE, resCol);
        canvas->drawString(resStr, 85, 75);
    }
    
    canvas->setTextColor(NEON_MAGENTA, BLACK);
    canvas->drawString(spinning ? "SPINNING..." : "UP:Color  <>:Bet  ENT:Spin", 5, 115);
    
    UI::push();
}

bool Roulette::isRunning() { return running; }