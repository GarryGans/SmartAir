#ifndef SWITCHERS_H
#define SWITCHERS_H

#include <Arduino.h>

#include <Key.h>
#include <Watch.h>

#define speedPinsAmount 2

#define OFF LOW
#define ON HIGH

class Switchers
{
    friend class Screen;

private:
    byte speedPins[speedPinsAmount];
    boolean flowRelay[speedPinsAmount];

    byte fanPin;

    byte fogPin;
    byte fogBut;
    boolean fogRelay;

public:
    Switchers(/* args */);
    ~Switchers();

    void begin(byte speedPins[], byte fanPin, byte fogPin, byte fogBut);
    void flowSwitcher(Key &key, Watch &watch);
    void fanSwitcher(Key &key, Watch &watch);
    void fogSwitcher(Key &key, Watch &watch);
};

#endif