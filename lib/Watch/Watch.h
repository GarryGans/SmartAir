#ifndef WATCH_H
#define WATCH_H

#include <Arduino.h>
#include <RTC_extention.h>
#include <Key.h>
#include <Timer.h>
#include <GyverButton.h>

#define speedPinsAmount 2
#define dayWork 10
#define dayPause 20
#define nightWork 2
#define nightPause 60

class Watch : public RTC_extention
{
    friend class Switchers;
    friend class Screen;

private:
    boolean fog;
    boolean onlyDay = 0;
    boolean firstStart;
    boolean night;
    boolean newDuration;

    byte flowPin;
    int morning = 60 * 9;
    int evening = 60 * 20;
    int hour = 60;

    int startDay;
    int finishDay;

    int work;
    int pause;

    int play;
    int stop;


    const int midNightBefore = 23 * 60 + 59;
    const int midNightAfter = 0;

    boolean flowSwitch[speedPinsAmount];
    boolean fanSwitch;
    boolean fogSwitch;

    int startHour = 5;
    int startMin = 00;
    int finishHour = 23;
    int finishMin = 10;

    int playHour;
    int playMin;
    int stopHour;
    int stophMin;

public:
    Watch(/* args */);
    ~Watch();

    void timeFromMinute(int time, int &hour, int &minute);

    int timeToSecond(int hour, int min, int sec);
    int timeToMinute(int hour, int min);

    int nowSec();
    int nowTime();

    byte speedPin();
    byte setFlowPin();
    void switchFlow();

    void midNigth(int &value);
    void correctWork();

    void calculateStop();
    void calculatePlay();
    void stopStart();

    void setDayNight();
    void calculateFlowSwitch();

    void autoFlow(Key &key);
    void autoFog(Key &key);
    void autoFan(Key &key);
};

#endif