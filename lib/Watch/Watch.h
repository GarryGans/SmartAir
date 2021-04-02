#ifndef WATCH_H
#define WATCH_H

#include <Arduino.h>
#include <RTC_extention.h>
#include <Key.h>

#define speedPinsAmount 2
#define dayWork 2
#define nightWork 1
#define dayPause 1
#define nightPause 3

class Watch : public RTC_extention
{
    friend class Switchers;
    friend class Screen;

private:
    boolean onlyDay = 1;
    boolean firstStart;

    int start;
    int finish;
    int work;
    int pause;

    int play;
    int stop;

    const int midNightBefore = 23 * 60 + 59;
    const int midNightAfter = 0;

    boolean autoSwitch[speedPinsAmount];

    int startHour = 23;
    int startMin = 58;
    int finishHour = 23;
    int finishMin = 59;

public:
    Watch(/* args */);
    ~Watch();

    void switchFlow(int play, int stop, boolean autoSwitch[]);
    void autoFlow(Key &key);
    int nowTime();
    int nowSec();
    int calculateTimeToSecond(int hour, int min, int sec);

    int calculateTimeToMinute(int hour, int min);
    void calculateAutoSwitch(int begin, int finish, int &start, int &stop, int &work, int &pause);
};

#endif