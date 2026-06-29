#include "VideoPoker.h"

int VideoPoker::dealCard() { return random(0, 52); }

void VideoPoker::init() { running=true; betAmount=10; state=0; cursor=0; }

void VideoPoker::evaluateHand() {
    int ranks[13] = {0}; int suits[4] = {0};
    for(int i=0; i<5; i++) { ranks[hand[i]%13]++; suits[hand[i]/13]++; }
    
    bool flush=false; for(int i=0; i<4; i++) if(suits[i]==5) flush=true;
    bool straight=false; int consec=0;
    for(int i=0; i<14; i++) {
        if(ranks[i%13]>0) consec++; else consec=0;
        if(consec==5) straight=true;
    }
    
    int pairs=0, three=0, four=0;
    bool jacksOrBetter = false;
    for(int i=0; i<13; i++) {
        if(ranks[i]==2) { pairs++; if(i==0 || i>9) jacksOrBetter=true; } // Eso je 0, J=10, Q=11, K=12
        if(ranks[i]==3) three++;
        if(ranks[i]==4) four++;
    }
    
    int mult = 0;
    if(flush && straight && ranks[0]>0 && ranks[12]>0) mult = 800; // Royal Flush
    else if(flush && straight) mult = 50;
    else if(four) mult = 25;
    else if(three && pairs) mult = 9; // Full House
    else if(flush) mult = 6;
    else if(straight) mult = 4;
    else if(three) mult = 3;
    else if(pairs==2) mult = 2;
    else if(jacksOrBetter) mult = 1;
    
    if (mult > 0) { 
        Save::addBalance(betAmount * mult); 
        if (mult > 4) { Audio::playJackpot(); UI::confetti(); } else { Audio::playWin(); }
    } else {
        UI::shake();
    }
}

void VideoPoker::update() {
    M5Cardputer.update();
    if(state==0) {
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { if(betAmount>10) betAmount-=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { if(betAmount+10<=Save::balance) betAmount+=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) && Save::balance>=betAmount) {
            Save::addBalance(-betAmount);
            for(int i=0; i<5; i++) { hand[i] = dealCard(); hold[i]=false; }
            state=1; cursor=0; Audio::playBeep(); delay(200);
        }
    } else if(state==1) { // Vybírání karet pro podržení
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { cursor=(cursor>0)?cursor-1:4; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { cursor=(cursor<4)?cursor+1:0; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(';')) { hold[cursor]=!hold[cursor]; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) { // Druhé rozdání
            for(int i=0; i<5; i++) { if(!hold[i]) hand[i] = dealCard(); }
            evaluateHand();
            state=2; delay(200);
        }
    } else {
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) state=0;
    }
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
}

void VideoPoker::draw() {
    auto canvas = UI::getCanvas(); canvas->fillScreen(BLACK); UI::drawHeader("VIDEO POKER", Save::balance);
    
    const char* rNames[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
    const char* sNames[] = {"H","D","C","S"}; // Hearts, Diamonds, Clubs, Spades
    uint16_t sColors[] = {RED, RED, NEON_CYAN, NEON_CYAN}; // Červené a "černé"
    
    if(state > 0) {
        for(int i=0; i<5; i++) {
            int px = 5 + (i*46);
            UI::drawNeonFrame(px, 35, 42, 55, (state==1 && cursor==i) ? WHITE : DARKGREY);
            
            canvas->setTextColor(sColors[hand[i]/13], BLACK);
            canvas->drawString(rNames[hand[i]%13], px+5, 45); // Hodnota
            canvas->drawString(sNames[hand[i]/13], px+25, 65); // Barva
            
            if (hold[i]) {
                canvas->setTextColor(NEON_GREEN, BLACK); canvas->drawString("HLD", px+5, 95);
            }
        }
    }
    
    if (state==0) { canvas->setTextColor(NEON_GREEN, BLACK); canvas->drawString((String("Bet: $") + betAmount).c_str(), 100, 60); }
    canvas->setTextColor(NEON_MAGENTA, BLACK); 
    canvas->drawString(state==0?"<>:Bet  ENT:Deal":state==1?"<>:Move  UP:Hold  ENT:Draw":"ENT: Play Again", 5, 115);
    UI::push();
}
bool VideoPoker::isRunning() { return running; }