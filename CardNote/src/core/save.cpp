#include "save.h"
#include <SPI.h>

// Nastavení startovních hodnot
int Save::balance = 1000;
int Save::highScore = 1000;
int Save::xp = 0;
int Save::level = 1;
int Save::debt = 0;
float Save::btcAmount = 0.0f;
float Save::btcAvgPrice = 0.0f;
int Save::history[50] = {0};
int Save::histCount = 0;
bool Save::perks[15] = {false};
int Save::stockShares[5] = {0};

void Save::init() {
    // Nasměrování SPI přímo na piny M5Cardputeru
    SPI.begin(40, 39, 14, 12);

    if (!SD.begin(12, SPI)) {
        Serial.println("SD Card Failed!");
        return;
    }

    // Pokud nemáme save, vytvoříme ho. Jinak načítáme.
    if (!SD.exists("/save.txt")) {
        saveAll();
    } else {
        loadAll();
    }
}

void Save::loadAll() {
    File file = SD.open("/save.txt", FILE_READ);
    if (!file) return;

    balance = file.readStringUntil('\n').toInt();
    highScore = file.readStringUntil('\n').toInt();
    xp = file.readStringUntil('\n').toInt();
    level = file.readStringUntil('\n').toInt();
    debt = file.readStringUntil('\n').toInt();
    btcAmount = file.readStringUntil('\n').toFloat();
    btcAvgPrice = file.readStringUntil('\n').toFloat();
    histCount = file.readStringUntil('\n').toInt();

    for (int i = 0; i < 50; i++) {
        history[i] = file.readStringUntil('\n').toInt();
    }
    for (int i = 0; i < 15; i++) {
        perks[i] = file.readStringUntil('\n').toInt();
    }
    for (int i = 0; i < 5; i++) {
        stockShares[i] = file.readStringUntil('\n').toInt();
    }
    file.close();
}

void Save::saveAll() {
    // Smazání starého souboru kvůli chybě přepisování na ESP32
    if (SD.exists("/save.txt")) {
        SD.remove("/save.txt");
    }

    File file = SD.open("/save.txt", FILE_WRITE);
    if (!file) return;

    file.println(balance);
    file.println(highScore);
    file.println(xp);
    file.println(level);
    file.println(debt);
    file.println(btcAmount);
    file.println(btcAvgPrice);
    file.println(histCount);

    for (int i = 0; i < 50; i++) {
        file.println(history[i]);
    }
    for (int i = 0; i < 15; i++) {
        file.println(perks[i]);
    }
    for (int i = 0; i < 5; i++) {
        file.println(stockShares[i]);
    }
    file.close();
}

void Save::pushHistory() {
    for (int i = 49; i > 0; i--) {
        history[i] = history[i - 1];
    }
    history[0] = balance;
    if (histCount < 50) histCount++;
}

void Save::addBalance(int amount) {
    balance += amount;
    if (balance > highScore) highScore = balance;
    
    if (amount != 0) {
        pushHistory();
        if (amount < 0) {
            addXP(abs(amount)); // XP za utracené
        } else {
            addXP(amount / 2);  // XP za získané
        }
    }
    saveAll();
}

void Save::addXP(int amount) {
    // VIP Card Perk z Black Marketu násobí XP
    if (perks[1]) {
        amount = amount * 1.5;
    }
    xp += amount;
    
    int nextLevelXP = level * 1000;
    while (xp >= nextLevelXP) {
        xp -= nextLevelXP;
        level++;
        nextLevelXP = level * 1000;
    }
}

void Save::takeLoan(int amount) {
    debt += amount;
    addBalance(amount);
}

void Save::payLoan(int amount) {
    if (balance >= amount && debt >= amount) {
        debt -= amount;
        addBalance(-amount);
    }
}