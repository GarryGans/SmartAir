#include "Switchers.h"

Switchers::Switchers(/* args */)
{
}

Switchers::~Switchers()
{
}

void Switchers::begin(byte speedPins[])
{
    for (byte i = 0; i < speedPinsAmount; i++)
    {
        this->speedPins[i] = speedPins[i];
        pinMode(speedPins[i], OUTPUT);
        digitalWrite(speedPins[i], OFF);
    }
}

void Switchers::switcher(Key &key, Watch &watch)
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