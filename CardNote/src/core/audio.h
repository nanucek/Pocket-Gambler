#pragma once
#include <M5Cardputer.h>

class Audio {
public:
    static bool enabled;
    static void init();
    static void toggle();
    static void playBeep();
    static void playWin();
    static void playJackpot();
};