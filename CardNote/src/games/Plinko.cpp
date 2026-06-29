#include "Plinko.h"

void Plinko::init() { running=true; betAmount=10; state=0; ballRow=0; ballCol=4; }

void Plinko::update() {
    M5Cardputer.update();
    if(state==0) {
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { if(betAmount>10) betAmount-=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { if(betAmount+10<=Save::balance) betAmount+=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) && Save::balance>=betAmount) {
            Save::addBalance(-betAmount);
            ballRow=0; ballCol=4; // Začátek uprostřed
            state=1; Audio::playBeep();
        }
    } else if(state==1) {
        // Animace pádu
        for(ballRow=0; ballRow<8; ballRow++) {
            ballCol += (random(0, 2) == 0) ? -1 : 1; // Padá doleva nebo doprava
            if (ballCol < 0) ballCol = 0; if (ballCol > 8) ballCol = 8;
            draw();
            Audio::playBeep();
            delay(150);
        }
        float mult = multipliers[ballCol];
        int win = betAmount * mult;
        Save::addBalance(win);
        
        if (mult >= 3.0) { Audio::playJackpot(); UI::confetti(); }
        else if (mult > 0.5) { Audio::playWin(); }
        else { UI::shake(); } // Prohra (násobitel menší než 1)
        
        state=2;
    } else {
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) state=0;
    }
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
}

void Plinko::draw() {
    auto canvas = UI::getCanvas(); canvas->fillScreen(BLACK); UI::drawHeader("NEON PLINKO", Save::balance);
    
    // Vykreslení sítě a kuličky
    for(int r=0; r<=8; r++) {
        for(int c=0; c<=8; c++) {
            int px = 50 + (c*16); int py = 30 + (r*8);
            if (state == 1 && r == ballRow && c == ballCol) {
                canvas->fillCircle(px, py, 4, WHITE); // Kulička
            } else {
                canvas->drawPixel(px, py, DARKGREY); // Kolíky
            }
        }
    }
    
    // Zobrazení finální pozice
    if (state==2) canvas->fillCircle(50 + (ballCol*16), 30 + (8*8), 4, NEON_GREEN);
    
    canvas->setTextColor(NEON_GREEN, BLACK); canvas->drawString((String("Bet: $") + betAmount).c_str(), 5, 100);
    
    // Ukazatel násobitelů dole
    canvas->setTextColor(WHITE, BLACK);
    canvas->drawString(state==2 ? (String("WIN: $") + (int)(betAmount * multipliers[ballCol])).c_str() : "Drop the ball!", 100, 100);
    
    canvas->setTextColor(NEON_MAGENTA, BLACK); canvas->drawString(state==0?"<>:Bet  ENT:Drop":"ENT:Play Again", 5, 115);
    UI::push();
}
bool Plinko::isRunning() { return running; }