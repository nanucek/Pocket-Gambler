#include "HighLow.h"
void HighLow::init() { running=true; betAmount=10; currentNum=random(1, 100); state=0; }
void HighLow::update() {
    M5Cardputer.update();
    if(state==0) {
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { if(betAmount>10) betAmount-=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { if(betAmount+10<=Save::balance) betAmount+=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(';')) { 
            if(Save::balance>=betAmount) { Save::addBalance(-betAmount); nextNum=random(1,100); if(nextNum>=currentNum) {Save::addBalance(betAmount*2); Audio::playWin();} state=1; delay(200); }
        }
        if (M5Cardputer.Keyboard.isKeyPressed('.')) { 
            if(Save::balance>=betAmount) { Save::addBalance(-betAmount); nextNum=random(1,100); if(nextNum<=currentNum) {Save::addBalance(betAmount*2); Audio::playWin();} state=1; delay(200); }
        }
    } else {
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) { currentNum=nextNum; state=0; delay(200); }
    }
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
}
void HighLow::draw() {
    auto canvas = UI::getCanvas(); canvas->fillScreen(BLACK); UI::drawHeader("HIGH / LOW", Save::balance);
    canvas->setTextColor(WHITE, BLACK); canvas->drawString((String("Current: ")+currentNum).c_str(), 10, 40);
    canvas->setTextColor(NEON_GREEN, BLACK); canvas->drawString((String("Bet: $") + betAmount).c_str(), 140, 40);
    if(state==1) { canvas->setTextColor(NEON_CYAN, BLACK); canvas->drawString((String("Next was: ")+nextNum).c_str(), 10, 70); }
    canvas->setTextColor(NEON_MAGENTA, BLACK); canvas->drawString(state==0?"UP:High DOWN:Low <>:Bet":"ENTER to continue", 5, 115);
    UI::push();
}
bool HighLow::isRunning() { return running; }