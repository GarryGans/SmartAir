#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

#include <Switchers.h>
#include <Key.h>
#include <Watch.h>

class Screen : public U8G2_SH1106_128X64_NONAME_1_HW_I2C
{
private:
    int cursor;
    byte cursorPosition;
    byte choise;

    const String set[2] = {"d&n", "day"};

    const String fanState[4] = {"OFF", "ON", "LOW", "HIGH"};
    const char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    const unsigned long blinkMillis = 700;
    unsigned long currentMillis;
    unsigned long prewCursorMillis;

public:
    Screen(/* args */);
    ~Screen();

    void showDigit(int value);
    boolean blinkReady();

    void highLighter(Key &key);

    void showBlink(int value);

    void showFanTime(Watch &watch, Key &key);

    void showBlinkStringTime(int hh, int mm);

    void showStringTime(int hh, int mm);
    void showStringNowTime(int hh, int mm, int ss);
    void showStringFanTime(Watch &watch);

    void showAlert();
    void bottomLine(Watch &watch, Key &key);
    void showFanState(Switchers &relayState, Key &key);
    void headerTime(Watch &watch);
    void headerDate(Watch &watch);
    void showHeaderScreen(Watch &watch);
    void fanScreen(Switchers &relayState, Key &key, Watch &watch);
};

#endif