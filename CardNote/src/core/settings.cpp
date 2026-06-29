#include "Settings.h"

int SettingsOS::brightness = 100;

void SettingsOS::init() {
    M5Cardputer.Display.setBrightness(brightness);
}

void SettingsOS::changeBrightness(int delta) {
    brightness += delta;
    if (brightness > 255) brightness = 255;
    if (brightness < 20) brightness = 20; 
    M5Cardputer.Display.setBrightness(brightness);
}