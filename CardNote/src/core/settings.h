#pragma once
#include <M5Cardputer.h>
#include "audio.h"

class SettingsOS {
public:
    static int brightness;
    
    static void init();
    static void changeBrightness(int delta);
};