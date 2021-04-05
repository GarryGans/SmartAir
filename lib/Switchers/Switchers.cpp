#include "Switchers.h"

Switchers::Switchers(/* args */)
{
}

Switchers::~Switchers()
{
}

void Switchers::begin(byte speedPins[], byte fogPin, byte fogBut)
{
    for (byte i = 0; i < speedPinsAmount; i++)
    {
        this->speedPins[i] = speedPins[i];
        pinMode(speedPins[i], OUTPUT);
        digitalWrite(speedPins[i], OFF);
    }

    this->fogPin = fogPin;
    pinMode(fogPin, OUTPUT);
    digitalWrite(fogPin, OFF);

    this->fogBut = fogBut;
    pinMode(fogBut, OUTPUT);
    digitalWrite(fogBut, OFF);
}

void Switchers::flowSwitcher(Key &key, Watch &watch)
{
    for (byte i = 0; i < speedPinsAmount; i++)
    {
        if (watch.autoSwitch[i] || key.manualSwitch[i])
        {
            digitalWrite(speedPins[i], ON);
            relaySW[i] = true;
        }
        else
        {
            digitalWrite(speedPins[i], OFF);
            relaySW[i] = false;
        }
    }
}

void Switchers::fogSwitcher(Key &key, Watch &watch)
{
    if (watch.fogSwitch || key.manualFog)
    {
        digitalWrite(fogPin, ON);
        // relaySW[2] = true;
    }
    else
    {
        digitalWrite(fogPin, OFF);
        // relaySW[2] = false;
    }

    if (watch.fogBut)
    {
        digitalWrite(fogBut, ON);
    }
    else
    {
        digitalWrite(fogBut, OFF);
    }
}