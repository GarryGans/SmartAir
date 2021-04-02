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
    boolean relaySW[speedPinsAmount];

public:
    Switchers(/* args */);
    ~Switchers();

    void begin(byte speedPins[]);
    void switcher(Key &key, Watch &watch);
};

#endif