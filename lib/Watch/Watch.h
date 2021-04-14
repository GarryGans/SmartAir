#ifndef WATCH_H
#define WATCH_H

#include <Arduino.h>
#include <RTC_extention.h>
#include <Key.h>
#include <Timer.h>

#define speedPinsAmount 2
#define dayWork 5
#define dayPause 10
#define nightWork 5
#define nightPause 30

class Watch : public RTC_extention
{
    friend class Switchers;
    friend class Screen;

private:
    boolean fog;
    boolean onlyDay = 1;
    boolean firstStart;
    boolean night;
    boolean newDay;

    byte flowPin = 0;

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

    void switchFlow(int play, int stop, boolean flowSwitch[]);
    void correctStop(int finish, int &play, int &stop);
    void midNigth(int &value);
    void stopStart(int start, int finish, int &play, int &stop, int &work, int &pause);
    void setDuration(int start, int finish, int &work, int &pause);
    void calculateFlowSwitch(int begin, int finish, int &start, int &stop, int &work, int &pause);
    void autoFlow(Key &key);
    void autoFog(Key &key);
    void autoFan(Key &key);
};

#endif