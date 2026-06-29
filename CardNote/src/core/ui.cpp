#include "ui.h"
#include "save.h" 

float UI::currentHighlightY = 35.0f;
LGFX_Sprite UI::canvas(&M5Cardputer.Display);

uint16_t UI::C_BG, UI::C_PRIM, UI::C_SEC, UI::C_TEXT, UI::C_ACCENT;
int UI::currentTheme = 0;

void UI::setTheme(int index) {
    currentTheme = index;
    if(index == 0) { // Neon Cyberpunk
        C_BG = BLACK; C_PRIM = 0x07FF; C_SEC = 0xF81F; C_ACCENT = 0x07E0; C_TEXT = WHITE;
    } else if(index == 1) { // Matrix Hacker
        C_BG = BLACK; C_PRIM = 0x07E0; C_SEC = 0x03E0; C_ACCENT = WHITE; C_TEXT = 0x07E0;
    } else if(index == 2) { // Blood Casino
        C_BG = 0x1000; C_PRIM = 0xF800; C_SEC = 0xFDA0; C_ACCENT = 0xFEE0; C_TEXT = WHITE;
    }
}

void UI::init() {
    M5Cardputer.Display.setRotation(1);
    canvas.createSprite(240, 135); 
    canvas.setTextFont(1);
    canvas.setTextSize(2);
    setTheme(0);
}

LGFX_Sprite* UI::getCanvas() { return &canvas; }
void UI::push() { canvas.pushSprite(0, 0); }

void UI::shake() {
    for(int i=0; i<8; i++) {
        canvas.pushSprite(random(-4, 5), random(-4, 5)); delay(30);
    }
    canvas.pushSprite(0, 0); 
}

void UI::confetti() {
    for(int frame=0; frame<20; frame++) {
        for(int i=0; i<40; i++) canvas.fillRect(random(240), random(25, 135), 3, 3, random(0xFFFF));
        canvas.pushSprite(0, 0); delay(40);
    }
}

void UI::drawHeader(const char* title, int balance) {
    canvas.fillRect(0, 0, 240, 25, C_BG);
    canvas.setTextColor(C_PRIM, C_BG);
    canvas.drawString(title, 5, 5);
    
    // Baterie posunuta, aby nezasahovala do penez
    int bat = M5.Power.getBatteryLevel();
    canvas.setTextColor(bat < 20 ? 0xF800 : C_SEC, C_BG); 
    canvas.drawRightString((String(bat) + "%").c_str(), 145, 5);
    
    char balStr[32];
    snprintf(balStr, sizeof(balStr), "$%d", balance);
    canvas.setTextColor(C_ACCENT, C_BG);
    canvas.drawRightString(balStr, 235, 5); 
    
    canvas.drawLine(0, 25, 240, 25, C_SEC);
}

void UI::drawNeonFrame(int x, int y, int w, int h, uint16_t color) {
    canvas.drawRect(x, y, w, h, color); canvas.drawRect(x+1, y+1, w-2, h-2, color);
}

void UI::drawMenu(const char** items, int itemCount, int selectedIndex) {
    int startY = 35; int itemHeight = 25;
    
    int startIdx = selectedIndex - 1;
    if (startIdx < 0) startIdx = 0;
    if (startIdx > itemCount - 4) startIdx = itemCount - 4;
    
    // Vypocet relativni pozice na viditelne obrazovce pro opraveny ramecek
    int visibleIndex = selectedIndex - startIdx;
    float targetY = startY + (visibleIndex * itemHeight);
    currentHighlightY += (targetY - currentHighlightY) * 0.2f; 
    
    canvas.fillScreen(C_BG);
    drawHeader("OS", Save::balance);
    
    drawNeonFrame(10, (int)currentHighlightY, 220, 22, C_SEC);
    
    for (int i = 0; i < 4; i++) {
        int idx = startIdx + i;
        if (idx < itemCount) {
            canvas.setTextColor(idx == selectedIndex ? C_TEXT : C_PRIM, C_BG);
            canvas.drawString(items[idx], 20, startY + (i * itemHeight) + 3);
        }
    }
    
    canvas.setTextColor(C_SEC, C_BG);
    canvas.drawRightString((String("LVL ") + Save::level).c_str(), 235, 115); 
    push();
}