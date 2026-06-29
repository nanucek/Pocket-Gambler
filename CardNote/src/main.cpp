#include <M5Cardputer.h>
#include "core/ui.h"
#include "core/save.h"
#include "core/audio.h"

// APLIKACE
#include "apps/PlayerCard.h"
#include "apps/Bank.h"
#include "apps/CryptoTrade.h"
#include "apps/BlackMarket.h"

// HRY
#include "games/Slots.h"
#include "games/Blackjack.h"
#include "games/Roulette.h"
#include "games/VideoPoker.h"
#include "games/Plinko.h"
#include "games/CoinFlip.h"
#include "games/Dice.h"
#include "games/HighLow.h"
#include "games/Crash.h"
#include "games/Shells.h"
#include "games/Mines.h"

// Promenne pro eventy
unsigned long lastEventTime = 0;
bool eventActive = false;
String eventTitle = "";
String eventDesc = "";

void triggerRandomEvent() {
    eventActive = true;
    int r = random(0, 10);
    switch(r) {
        case 0: eventTitle="LOTTERY!"; eventDesc="You won $500!"; Save::addBalance(500); break;
        case 1: eventTitle="MUGGED!"; eventDesc="Lost $200 in alley."; Save::addBalance(-200); break;
        case 2: eventTitle="BANK ERROR"; eventDesc="Debt reduced by $500."; if(Save::debt>500) Save::debt-=500; break;
        case 3: eventTitle="FOUND WALLET"; eventDesc="+ $150 cash."; Save::addBalance(150); break;
        case 4: eventTitle="POLICE RAID"; if(Save::perks[3]){ eventDesc="Bribed cop. You safe."; } else { eventDesc="Fined $400!"; Save::addBalance(-400); } break;
        case 5: eventTitle="BULL RUN"; eventDesc="Crypto is booming!"; Save::btcAvgPrice *= 0.8; break;
        case 6: eventTitle="BEAR MARKET"; eventDesc="Crypto crashed!"; Save::btcAmount *= 0.8; break;
        case 7: eventTitle="HACKED!"; eventDesc="Lost 100 XP."; Save::xp -= 100; if(Save::xp<0) Save::xp=0; break;
        case 8: eventTitle="GOOD DEED"; eventDesc="Helped lady. +500 XP."; Save::addXP(500); break;
        case 9: eventTitle="TAX MAN"; if(Save::perks[2]){ eventDesc="Tax evaded!";} else { eventDesc="Paid $300 taxes."; Save::addBalance(-300); } break;
    }
    Save::saveAll();
}

// MENU - 15 POLOŽEK
const int MENU_ITEMS = 15; 
const char* menu[MENU_ITEMS] = {
    "1. Player Card", 
    "2. Mafia Bank", 
    "3. Crypto Terminal", 
    "4. Black Market", 
    "5. Slot Machine",
    "6. Blackjack",
    "7. Roulette",
    "8. Video Poker",
    "9. Plinko",
    "10. Coin Flip",
    "11. Dice",
    "12. High/Low",
    "13. Crash",
    "14. Shells",
    "15. Mines"
};

int selected = 0;
Game* activeApp = nullptr;

void setup() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg); 
    
    UI::init();
    Audio::init();
    Save::init();
    
    M5Cardputer.update(); 
    lastEventTime = millis();
    randomSeed(esp_random()); 
}

void loop() {
    // Rate limiter pro sběrnici klávesnice (100 Hz)
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate < 10) return;
    lastUpdate = millis();

    M5Cardputer.update();

    // Vlastní hardwarový debounce kláves
    auto status = M5Cardputer.Keyboard.keysState();
    
    bool currentEnter = status.enter || M5Cardputer.BtnA.isPressed();
    bool currentUp = false;
    bool currentDown = false;
    
    for (char c : status.word) {
        if (c == ';') currentUp = true;
        if (c == '.') currentDown = true;
        if (c == '\r' || c == '\n') currentEnter = true;
    }

    static bool lastEnter = false;
    static bool lastUp = false;
    static bool lastDown = false;

    bool btnEnter = currentEnter && !lastEnter;
    bool btnUp = currentUp && !lastUp;
    bool btnDown = currentDown && !lastDown;

    lastEnter = currentEnter;
    lastUp = currentUp;
    lastDown = currentDown;

    // 1. Obsluha běžící aplikace
    if (activeApp != nullptr) {
        activeApp->update();
        if (activeApp->isRunning()) {
            activeApp->draw();
        } else {
            delete activeApp;
            activeApp = nullptr;
            UI::getCanvas()->fillScreen(UI::C_BG);
        }
        return;
    }

    // 2. Časovač eventů
    if (millis() - lastEventTime > 60000 && !eventActive) {
        if(random(0, 100) > 70) triggerRandomEvent();
        lastEventTime = millis();
    }

    // 3. Vykreslení náhodného eventu (vyčištěné překrývání)
    if(eventActive) {
        auto canvas = UI::getCanvas();
        canvas->fillScreen(UI::C_BG);
        UI::drawHeader("SYSTEM EVENT", Save::balance);
        
        UI::drawNeonFrame(20, 30, 200, 75, 0xF800); 
        canvas->fillRect(22, 32, 196, 71, 0x0000);  
        canvas->setTextColor(0xFFE0, 0x0000);       
        canvas->drawString(eventTitle.c_str(), 30, 42);
        canvas->setTextColor(0xFFFF, 0x0000);       
        canvas->drawString(eventDesc.c_str(), 30, 65);
        canvas->drawString("Press ENTER / G0", 50, 85);
        UI::push();
        
        if(btnEnter) {
            eventActive = false; 
            lastEventTime = millis(); 
        }
        return; 
    }

    // 4. Skrolování v menu
    if (btnUp) { 
        if (selected > 0) selected--;
    }
    if (btnDown) { 
        if (selected < MENU_ITEMS - 1) selected++;
    }

    // 5. Spuštění vybrané aplikace z menu (15 položek)
    if (btnEnter) {
        switch(selected) {
            case 0: activeApp = new PlayerCard(); break;
            case 1: activeApp = new Bank(); break;
            case 2: activeApp = new CryptoTrade(); break;
            case 3: activeApp = new BlackMarket(); break;
            case 4: activeApp = new Slots(); break;
            case 5: activeApp = new Blackjack(); break;
            case 6: activeApp = new Roulette(); break;
            case 7: activeApp = new VideoPoker(); break;
            case 8: activeApp = new Plinko(); break;
            case 9: activeApp = new CoinFlip(); break;
            case 10: activeApp = new Dice(); break;
            case 11: activeApp = new HighLow(); break;
            case 12: activeApp = new Crash(); break;
            case 13: activeApp = new Shells(); break;
            case 14: activeApp = new Mines(); break;
        }
        if (activeApp != nullptr) {
            activeApp->init();
        }
    }

    // 6. Vykreslení hlavního menu (opravený Z-index a odsazení rámečku)
    auto canvas = UI::getCanvas();
    canvas->fillScreen(UI::C_BG);
    
    UI::drawHeader("OS", Save::balance);

    int startIdx = selected - 1;
    if (startIdx < 0) startIdx = 0;
    if (startIdx > MENU_ITEMS - 4) startIdx = max(0, MENU_ITEMS - 4);
    
    // Rámeček se kreslí první, roztažený tak, aby neřezal text
    int frameY = 35 + ((selected - startIdx) * 22) - 3;
    UI::drawNeonFrame(8, frameY, 224, 24, UI::C_PRIM);

    // Texty se kreslí až přes rámeček (nebo mimo něj) s odsazením zleva
    for (int i = 0; i < 4; i++) {
        int idx = startIdx + i;
        if (idx < MENU_ITEMS) {
            canvas->setTextColor(idx == selected ? UI::C_TEXT : UI::C_SEC);
            canvas->drawString(menu[idx], 25, 35 + (i * 22));
        }
    }
    
    UI::push();
}