#include "CoinFlip.h"
void CoinFlip::init() { running=true; betAmount=10; choice=0; state=0; }
void CoinFlip::update() {
    M5Cardputer.update();
    if (state == 0) {
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { if(betAmount>10) betAmount-=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { if(betAmount+10<=Save::balance) betAmount+=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(';')) { choice = !choice; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) && Save::balance>=betAmount) {
            Save::addBalance(-betAmount);
            state = 1; Audio::playBeep();
        }
    } else if (state == 1) { // Flipping effect
        for(int i=0; i<10; i++) { result = random(0, 2); draw(); delay(50); }
        if (result == choice) { Save::addBalance(betAmount*2); Audio::playWin(); }
        state = 2;
    } else {
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) state = 0;
    }
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
}
void CoinFlip::draw() {
    auto canvas = UI::getCanvas(); canvas->fillScreen(BLACK); UI::drawHeader("COIN FLIP (2x)", Save::balance);
    if(state==0) {
        canvas->setTextColor(WHITE, BLACK); canvas->drawString(choice==0?"Select: HEADS":"Select: TAILS", 70, 50);
        canvas->setTextColor(NEON_GREEN, BLACK); canvas->drawString((String("Bet: $") + betAmount).c_str(), 80, 80);
    } else {
        canvas->setTextColor(WHITE, BLACK); canvas->drawString(result==0?"Result: HEADS":"Result: TAILS", 70, 50);
        canvas->setTextColor(result==choice?NEON_GREEN:RED, BLACK); canvas->drawString(result==choice?"YOU WIN!":"YOU LOSE", 80, 80);
    }
    canvas->setTextColor(NEON_MAGENTA, BLACK); canvas->drawString(state==0?"UP:Flip  <>:Bet  ENT:Play":"ENT:Play Again", 5, 115);
    UI::push();
}
bool CoinFlip::isRunning() { return running; }