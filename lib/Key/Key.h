#ifndef KEY_H
#define KEY_H

#include <Arduino.h>
#include <AmperkaKB.h>

#define speedPinsAmount 2

class Key : public AmperkaKB
{
    friend class Screen;
    friend class Switchers;
    friend class Watch;

private:
    enum Buttons
    {

        PLUS = 1,
        MINUS = 2,
        SELECT = 3,
        MODE = 4

    };

    enum
    {
        TIME,
        DATE,
        START,
        STOP

    } highLight;

    enum Mode
    {
        AUTO,
        LIGHT,
        MANUAL
    } mode;

    enum FanModes
    {
        LOW_SPEED,
        HIGH_SPEED
    } fanMode = LOW_SPEED;

    enum FanState
    {
        swOFF,
        swON
    } fanState;

    boolean dayReduration;
    boolean manualFlow[speedPinsAmount];
    boolean manualFan;
    boolean manualFog;

public:
    Key(byte pin[]);
    ~Key();

    void select();
    void keyCommands();
    void manualSwitcher();
};

#endif