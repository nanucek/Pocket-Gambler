#include "Crash.h"
void Crash::init() { running=true; betAmount=10; mult=1.0f; state=0; }
void Crash::update() {
    M5Cardputer.update();
    if(state==0) {
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { if(betAmount>10) betAmount-=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { if(betAmount+10<=Save::balance) betAmount+=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) && Save::balance>=betAmount) {
            Save::addBalance(-betAmount); crashPoint = 1.0f + (random(0, 500) / 100.0f); 
            mult=1.0f; state=1; Audio::playBeep();
        }
    } else if(state==1) {
        mult += 0.05f; draw(); delay(100);
        if(mult >= crashPoint) { state=3; Audio::playBeep(); }
        else if (M5Cardputer.Keyboard.isKeyPressed(';')) { Save::addBalance(betAmount * mult); state=2; Audio::playWin(); }
    } else {
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) state=0;
    }
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
}
void Crash::draw() {
    auto canvas = UI::getCanvas(); canvas->fillScreen(BLACK); UI::drawHeader("CRASH", Save::balance);
    canvas->setTextColor(WHITE, BLACK); canvas->drawString((String("x") + mult).c_str(), 100, 50);
    canvas->setTextColor(NEON_GREEN, BLACK); canvas->drawString((String("Bet: $") + betAmount).c_str(), 10, 80);
    if(state==2) { canvas->setTextColor(NEON_GREEN, BLACK); canvas->drawString("CASHED OUT!", 100, 80); }
    if(state==3) { canvas->setTextColor(RED, BLACK); canvas->drawString("CRASHED!", 100, 80); }
    canvas->setTextColor(NEON_MAGENTA, BLACK); canvas->drawString(state==0?"ENT:Start <>:Bet":state==1?"UP: CASHOUT":"ENT:Play Again", 5, 115);
    UI::push();
}
bool Crash::isRunning() { return running; }