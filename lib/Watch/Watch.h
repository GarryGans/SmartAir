#ifndef WATCH_H
#define WATCH_H

#include <Arduino.h>
#include <RTC_extention.h>
#include <Key.h>
#include <Timer.h>

#define speedPinsAmount 2
#define dayWork 10
#define dayPause 0
#define nightWork 3
#define nightPause 30

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
    int morning = 60 * 8;
    int evening = 60 * 22;
    int hour = 60;

    int startDay;
    int finishDay;

    int work;
    int pause;

    int play;
    int stop;

    int stopFog;
    int fogTime = 1;

    const int midNightBefore = 23 * 60 + 59;
    const int midNightAfter = 0;

    boolean flowSwitch[speedPinsAmount];
    boolean fanSwitch;
    boolean fogSwitch;
    boolean fogBut;

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

    byte setFlowPin();
    void switchFlow(int play, int stop, boolean flowSwitch[]);
    void midNigth(int &value);
    void correctWork(int startDay, int finishDay, int play, int &work);
    void calculateStop(int startDay, int finishDay, int play, int &stop, int &work, int pause);
    void calculatePlay(int startDay, int &play, int stop, int pause);
    void stopStart(int startDay, int finishDay, int &play, int &stop, int &work, int pause);
    void setDayNight(int startDay, int finishDay, int &work, int &pause);
    void calculateFlowSwitch(int startDay, int finishDay, int &start, int &stop, int &work, int &pause);
    void autoFlow(Key &key);
    void autoFog(Key &key);
    void autoFan(Key &key);
};

#endif