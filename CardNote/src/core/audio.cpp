#include "audio.h"

bool Audio::enabled = true;

void Audio::init() {
    M5Cardputer.Speaker.begin();
    M5Cardputer.Speaker.setVolume(100);
}

void Audio::toggle() {
    enabled = !enabled;
    if (enabled) playBeep();
}

void Audio::playBeep() {
    if (!enabled) return;
    M5Cardputer.Speaker.tone(2000, 50);
}

void Audio::playWin() {
    if (!enabled) return;
    M5Cardputer.Speaker.tone(1500, 100);
    delay(100);
    M5Cardputer.Speaker.tone(2500, 200);
}

void Audio::playJackpot() {
    if (!enabled) return;
    for (int i = 0; i < 5; i++) {
        M5Cardputer.Speaker.tone(3000, 100);
        delay(100);
        M5Cardputer.Speaker.tone(4000, 100);
        delay(100);
    }
}
