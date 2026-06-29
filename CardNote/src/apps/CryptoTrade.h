#pragma once
#include "../core/Game.h"
#include "../core/ui.h"
#include "../core/save.h"
#include <WiFi.h>
#include <HTTPClient.h>

class CryptoTrade : public Game {
private:
    bool running; 
    int state; 
    float currentBTC; 
    String statusMsg;
    
    String ssidList[4];
    int numNetworks;
    int selectedSSID;
    String currentPassword;
    String selectedSSIDName;

    void fetchBTC() {
        if(WiFi.status() != WL_CONNECTED) return;
        HTTPClient http;
        http.begin("https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT");
        int httpCode = http.GET();
        if(httpCode == 200) {
            String payload = http.getString();
            int idx = payload.indexOf("\"price\":\"") + 9;
            String priceStr = payload.substring(idx, payload.indexOf("\"", idx));
            currentBTC = priceStr.toFloat();
            statusMsg = "Live Feed Connected.";
        } else { statusMsg = "API Error!"; }
        http.end();
    }

    void scanWiFi() {
        numNetworks = WiFi.scanNetworks();
        if (numNetworks > 4) numNetworks = 4;
        for (int i = 0; i < numNetworks; i++) {
            ssidList[i] = WiFi.SSID(i);
        }
        if (numNetworks > 0) state = 1;
        else statusMsg = "No WiFi found. DEL to exit.";
    }

public:
    void init() override {
        running = true; state = 0; currentBTC = 0; 
        numNetworks = 0; selectedSSID = 0; currentPassword = "";
        WiFi.mode(WIFI_STA); WiFi.disconnect();
        statusMsg = "Scanning WiFi...";
    }

    void update() override {
        M5Cardputer.update();
        
        if(state == 0) {
            draw(); 
            scanWiFi();
        } 
        else if(state == 1) { 
            if(M5Cardputer.Keyboard.isKeyPressed(';')) { selectedSSID--; if(selectedSSID<0) selectedSSID=numNetworks-1; delay(150); }
            if(M5Cardputer.Keyboard.isKeyPressed('.')) { selectedSSID=(selectedSSID+1)%numNetworks; delay(150); }
            if(M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
                selectedSSIDName = ssidList[selectedSSID];
                currentPassword = "";
                state = 2; delay(200);
            }
        } 
        else if(state == 2) { 
            if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
                auto status = M5Cardputer.Keyboard.keysState();
                for (char c : status.word) { currentPassword += c; }
                if (status.del && currentPassword.length() > 0) currentPassword.remove(currentPassword.length() - 1);
                if (status.enter) { state = 3; delay(200); }
            }
        } 
        else if(state == 3) { 
            statusMsg = "Connecting..."; draw();
            WiFi.begin(selectedSSIDName.c_str(), currentPassword.c_str());
            int attempts = 0;
            while(WiFi.status() != WL_CONNECTED && attempts < 15) { delay(500); attempts++; }
            if(WiFi.status() == WL_CONNECTED) { fetchBTC(); state = 4; } 
            else { statusMsg = "WiFi Failed! Wrong pass?"; state = 1; delay(2000); }
        } 
        else if(state == 4) { 
            if(M5Cardputer.Keyboard.isKeyPressed('u')) { fetchBTC(); delay(500); }
            if(M5Cardputer.Keyboard.isKeyPressed('b') && Save::balance >= 100 && currentBTC > 0) { 
                Save::addBalance(-100);
                float btcBought = 100.0f / currentBTC;
                Save::btcAvgPrice = ((Save::btcAmount * Save::btcAvgPrice) + 100.0f) / (Save::btcAmount + btcBought);
                Save::btcAmount += btcBought;
                Save::saveAll();
                statusMsg = "Bought $100 of BTC!";
                delay(200);
            }
            if(M5Cardputer.Keyboard.isKeyPressed('s') && Save::btcAmount > 0 && currentBTC > 0) { 
                int fiatValue = Save::btcAmount * currentBTC;
                Save::addBalance(fiatValue);
                Save::btcAmount = 0; Save::btcAvgPrice = 0;
                Save::saveAll();
                statusMsg = "Sold all BTC!";
                delay(200);
            }
        }
        
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE)) { WiFi.disconnect(true); running = false; }
    }

    void draw() override {
        auto canvas = UI::getCanvas(); canvas->fillScreen(UI::C_BG); UI::drawHeader("BINANCE", Save::balance);
        
        if(state == 0 || state == 3) { 
            canvas->setTextColor(UI::C_PRIM, UI::C_BG); canvas->drawString(statusMsg.c_str(), 10, 60); 
        } 
        else if(state == 1) {
            canvas->setTextColor(UI::C_SEC, UI::C_BG); canvas->drawString("Select Wi-Fi:", 10, 30);
            for(int i = 0; i < numNetworks; i++) {
                canvas->setTextColor(i == selectedSSID ? UI::C_TEXT : UI::C_PRIM, UI::C_BG);
                canvas->drawString(ssidList[i].c_str(), 20, 50 + (i * 16));
            }
        } 
        else if(state == 2) {
            canvas->setTextColor(UI::C_SEC, UI::C_BG); canvas->drawString((String("Pass: ") + selectedSSIDName).c_str(), 10, 35);
            UI::drawNeonFrame(10, 55, 220, 25, UI::C_PRIM);
            canvas->setTextColor(UI::C_TEXT, UI::C_BG); 
            canvas->drawString(currentPassword.c_str(), 15, 60);
            canvas->setTextColor(UI::C_PRIM, UI::C_BG); canvas->drawString("Type pass & ENTER", 5, 115);
        } 
        else if(state == 4) {
            canvas->setTextColor(UI::C_SEC, UI::C_BG); canvas->drawString((String("BTC: $") + currentBTC).c_str(), 10, 35);
            canvas->setTextColor(UI::C_TEXT, UI::C_BG);
            canvas->drawString((String("Wallet: ") + Save::btcAmount + " BTC").c_str(), 10, 60);
            
            if(Save::btcAmount > 0) {
                float profit = (currentBTC - Save::btcAvgPrice) * Save::btcAmount;
                canvas->setTextColor(profit >= 0 ? 0x07E0 : 0xF800, UI::C_BG); // Bezpecny hex bez maker
                canvas->drawString((String("P/L: $") + profit).c_str(), 10, 80);
            }
            canvas->setTextColor(UI::C_PRIM, UI::C_BG);
            canvas->drawString("u:Update  b:Buy $100  s:Sell All", 5, 115);
        }
        UI::push();
    }

    bool isRunning() override { return running; }
};