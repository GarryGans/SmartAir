#include "Switchers.h"

Switchers::Switchers(/* args */)
{
}

Switchers::~Switchers()
{
}

void Switchers::begin(byte speedPins[], byte fanPin, byte fogPin, byte fogBut)
{
    for (byte i = 0; i < speedPinsAmount; i++)
    {
        this->speedPins[i] = speedPins[i];
        pinMode(speedPins[i], OUTPUT);
        digitalWrite(speedPins[i], OFF);
    }

    this->fanPin = fanPin;
    pinMode(fanPin, OUTPUT);
    digitalWrite(fanPin, OFF);

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
        if (watch.flowSwitch[i] || key.manualFlow[i])
        {
            digitalWrite(speedPins[i], ON);
            flowRelay[i] = true;
        }
        else
        {
            digitalWrite(speedPins[i], OFF);
            flowRelay[i] = false;
        }
    }
}

void Switchers::fanSwitcher(Key &key, Watch &watch)
{
    if (watch.fanSwitch || key.manualFan)
    {
        digitalWrite(fanPin, ON);
    }
    else
    {
        digitalWrite(fanPin, OFF);
    }
}

void Switchers::fogSwitcher(Key &key, Watch &watch)
{
    if (watch.fogSwitch || key.manualFog)
    {
        digitalWrite(fogPin, ON);
        fogRelay = true;
    }
    else
    {
        digitalWrite(fogPin, OFF);
        fogRelay = false;
    }
}