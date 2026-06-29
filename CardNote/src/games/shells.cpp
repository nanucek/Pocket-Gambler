#include "Shells.h"
void Shells::init() { running=true; betAmount=10; selected=0; state=0; }
void Shells::update() {
    M5Cardputer.update();
    if(state==0 || state==2) {
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { if(betAmount>10) betAmount-=10; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { if(betAmount+10<=Save::balance) betAmount+=10; delay(150); }
        if(state==2 && M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) state=0;
        if(state==0 && M5Cardputer.Keyboard.isKeyPressed(';')) { 
            if(Save::balance>=betAmount) { Save::addBalance(-betAmount); ballPos=random(0,3); state=1; Audio::playBeep(); delay(200); }
        }
    } else if(state==1) {
        if (M5Cardputer.Keyboard.isKeyPressed(',')) { selected=(selected>0)?selected-1:2; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed('/')) { selected=(selected<2)?selected+1:0; delay(150); }
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
            if(selected==ballPos) { Save::addBalance(betAmount*3); Audio::playWin(); } else Audio::playBeep();
            state=2; delay(200);
        }
    }
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) running = false;
}
void Shells::draw() {
    auto canvas = UI::getCanvas(); canvas->fillScreen(BLACK); UI::drawHeader("SHELLS (3x)", Save::balance);
    for(int i=0; i<3; i++) {
        uint16_t c = (state==1 && selected==i) ? WHITE : NEON_CYAN;
        UI::drawNeonFrame(30 + i*60, 50, 40, 40, c);
        if(state==2 && ballPos==i) { canvas->setTextColor(NEON_GREEN, BLACK); canvas->drawString("O", 43+i*60, 62); }
    }
    canvas->setTextColor(NEON_GREEN, BLACK); canvas->drawString((String("Bet: $") + betAmount).c_str(), 10, 95);
    canvas->setTextColor(NEON_MAGENTA, BLACK); 
    canvas->drawString(state==0?"UP:Hide Ball <>:Bet":state==1?"<>:Select  ENT:Reveal":"ENT: Play Again", 5, 115);
    UI::push();
}
bool Shells::isRunning() { return running; }