#ifndef WATCH_H
#define WATCH_H

#include <Arduino.h>
#include <RTC_extention.h>
#include <Key.h>
#include <Timer.h>
#include <GyverButton.h>

#define speedPinsAmount 2
#define dayWork 10
#define dayPause 10
#define nightWork 2
#define nightPause 30

class Watch : public RTC_extention
{
    friend class Switchers;
    friend class Screen;

private:
    boolean fog;
    boolean onlyDay;
    boolean firstStart;
    boolean night;
    boolean newDuration;

    byte flowPin;
    int morning = 60 * 6;
    int evening = 60 * 21;
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
    int finishMin = 30;

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
    void switchFlow();

    int midNigth(int value);
    int next(int day, int work);
    void correctWork();
    int correctStop(int stop);

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