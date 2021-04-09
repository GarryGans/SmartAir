#ifndef TIMER_H
#define TIMER_H

#define couterAddr 0

#include <Arduino.h>
#include <EEPROM.h>

class Timer
{
    // friend class Screen;
    friend class Watch;

private:
    boolean blink;
    boolean move;
    boolean escBar;
    boolean blinkHide;
    boolean setTimerFlag;
    boolean startTimer;

    const byte maxEscapeCounter = 5;
    byte escapeCounter = maxEscapeCounter;
    const byte maxUnblockCounter = 2;
    byte unblockCounter = maxUnblockCounter;
    byte counter;

    const byte defaultCounter = 10;
    const byte minSetCounter = 1;
    const byte maxSetCounter = 99;

    const unsigned long secMillis = 1000;
    const unsigned long blinkMillis = 200;
    unsigned long prewCounterMillis;
    unsigned long prewCursorMillis;
    unsigned long prewMoveMillis;
    unsigned long prewEscapeMillis;

    unsigned long prewScreenMillis;
    unsigned long displayMillis = 3000;

    unsigned long prewBlinkMillis;

    byte maxCounter = 5;

    byte unfrizeCounter = maxCounter;

public:
    Timer(/* args */);
    ~Timer();

    void maxEscape();
    void maxUnblock();

    boolean moveReady();
    boolean blinkReady();
    void resetEscape();
    void resetTimer();
    boolean reduceTimer(byte &counter);
    void startEscSet();
    void changeTimer(boolean minus, boolean plus);
    void readTimer();
    void writeTimer();

    void minusCounter(byte &counter);
    boolean wait(unsigned long &prewMillis, unsigned long setMillis);
    boolean unfrize(byte &counter);
    void resetCounter(byte &counter);
};

#endif