#pragma once
#include <M5Cardputer.h>

// Zpětná kompatibilita pro starší vestavěné hry
#define NEON_CYAN    0x07FF
#define NEON_MAGENTA 0xF81F
#define NEON_DARK    0x10A2
#define NEON_GREEN   0x07E0

class UI {
private:
    static LGFX_Sprite canvas;
public:
    static float currentHighlightY;
    
    // Globální proměnné pro dynamický systém témat
    static uint16_t C_BG, C_PRIM, C_SEC, C_TEXT, C_ACCENT;
    static int currentTheme;
    static void setTheme(int index);
    
    static void init();
    static void drawHeader(const char* title, int balance);
    static void drawNeonFrame(int x, int y, int w, int h, uint16_t color);
    static void drawMenu(const char** items, int itemCount, int selectedIndex);
    
    static LGFX_Sprite* getCanvas();
    static void push();
    
    // Vizuální efekty pro výhry a prohry
    static void shake();
    static void confetti();
};