#pragma once

#include <M5Cardputer.h>
#include <FS.h>
#include <SD.h>

class Save {
public:
    // Původní základ
    static int balance, highScore, xp, level, debt;
    
    // Kompletní výbava pro v0.2.2
    static float btcAmount, btcAvgPrice;
    static int history[50];
    static int histCount;
    static bool perks[15];
    static int stockShares[5];

    static void init();
    static void loadAll();
    static void saveAll();
    static void addBalance(int amount);
    static void addXP(int amount);
    static void takeLoan(int amount);
    static void payLoan(int amount);
    static void pushHistory();
};