#include "Blackjack.h"

void Blackjack::init() {
    running = true; state = 0; betAmount = 10;
    pScore = 0; dScore = 0;
}

void Blackjack::dealCards() {
    Save::addBalance(-betAmount);
    pScore = random(2, 11) + random(2, 11);
    dScore = random(2, 11); 
    state = 1;
    Audio::playBeep();
}

void Blackjack::hit() {
    pScore += random(2, 11);
    Audio::playBeep();
    if (pScore > 21) {
        state = 2; 
    }
}

void Blackjack::stand() {
    while (dScore < 17) {
        dScore += random(2, 11);
        draw();
        delay(500);
    }
    
    if (dScore > 21 || pScore > dScore) {
        Save::addBalance(betAmount * 2); 
        Audio::playWin();
    } else if (pScore == dScore) {
        Save::addBalance(betAmount); 
    }
    state = 2;
}

void Blackjack::update() {
    M5Cardputer.update();
    
    if (state == 0) { 
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { if(betAmount > 10) betAmount -= 10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { if(betAmount + 10 <= Save::balance) betAmount += 10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) && Save::balance >= betAmount) dealCards();
    } else if (state == 1) { 
        if (M5Cardputer.Keyboard.isKeyPressed(';')) { hit(); delay(200); } 
        if (M5Cardputer.Keyboard.isKeyPressed('.')) { stand(); delay(200); } 
    } else if (state == 2) { 
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
            state = 0; 
            pScore = 0; dScore = 0;
            delay(200);
        }
    }
    
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
}

void Blackjack::draw() {
    auto canvas = UI::getCanvas();
    canvas->fillScreen(BLACK);
    UI::drawHeader("NEON BLACKJACK", Save::balance);
    
    if (state == 0) {
        char betStr[32];
        snprintf(betStr, sizeof(betStr), "Bet: $%d", betAmount);
        canvas->setTextColor(WHITE, BLACK);
        canvas->drawString(betStr, 80, 50);
        canvas->setTextColor(NEON_MAGENTA, BLACK);
        canvas->drawString("< > to Bet / ENTER to Deal", 10, 115);
    } else {
        char pStr[32], dStr[32];
        snprintf(dStr, sizeof(dStr), "Dealer: %d", dScore);
        snprintf(pStr, sizeof(pStr), "You: %d", pScore);
        
        canvas->setTextColor(RED, BLACK);
        canvas->drawString(dStr, 20, 40);
        canvas->setTextColor(NEON_CYAN, BLACK);
        canvas->drawString(pStr, 20, 70);
        
        canvas->setTextColor(NEON_MAGENTA, BLACK);
        if (state == 1) canvas->drawString("UP: Hit  /  DOWN: Stand", 10, 115);
        if (state == 2) canvas->drawString(pScore > 21 ? "BUST! Enter to restart" : "GAME OVER! Enter to restart", 10, 115);
    }
    
    UI::push();
}

bool Blackjack::isRunning() { return running; }