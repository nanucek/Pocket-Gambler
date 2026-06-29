#include "Dice.h"
void Dice::init() { running=true; betAmount=10; choice=0; state=0; d1=1; d2=1; }
void Dice::update() {
    M5Cardputer.update();
    if(state==0) {
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { if(betAmount>10) betAmount-=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { if(betAmount+10<=Save::balance) betAmount+=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(';')) { choice = (choice+1)%3; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) && Save::balance>=betAmount) {
            Save::addBalance(-betAmount);
            for(int i=0;i<10;i++){ d1=random(1,7); d2=random(1,7); draw(); delay(50); }
            int sum = d1+d2;
            if ((choice==0 && sum<7) || (choice==2 && sum>7)) { Save::addBalance(betAmount*2); Audio::playWin(); }
            else if (choice==1 && sum==7) { Save::addBalance(betAmount*4); Audio::playJackpot(); }
            state=1;
        }
    } else if(M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) state=0;
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
}
void Dice::draw() {
    auto canvas = UI::getCanvas(); canvas->fillScreen(BLACK); UI::drawHeader("NEON DICE", Save::balance);
    const char* opts[] = {"UNDER 7 (2x)", "EXACT 7 (4x)", "OVER 7 (2x)"};
    canvas->setTextColor(NEON_CYAN, BLACK); canvas->drawString(opts[choice], 10, 40);
    canvas->setTextColor(NEON_GREEN, BLACK); canvas->drawString((String("Bet: $") + betAmount).c_str(), 10, 65);
    if(state==1) {
        canvas->setTextColor(WHITE, BLACK); canvas->drawString((String("Roll: ")+d1+" + "+d2+" = "+(d1+d2)).c_str(), 100, 50);
    }
    canvas->setTextColor(NEON_MAGENTA, BLACK); canvas->drawString("UP:Pick  <>:Bet  ENT:Roll", 5, 115);
    UI::push();
}
bool Dice::isRunning() { return running; }