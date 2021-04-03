#ifndef WATCH_H
#define WATCH_H

#include <Arduino.h>
#include <RTC_extention.h>
#include <Key.h>

#define speedPinsAmount 2
#define dayWork 5
#define nightWork 5
#define dayPause 20
#define nightPause 60

class Watch : public RTC_extention
{
    friend class Switchers;
    friend class Screen;

private:
    boolean flowPin = 0;
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

    void switchFlow(int play, int stop, boolean autoSwitch[]);
    void autoFlow(Key &key);
    int nowTime();
    int nowSec();
    int calculateTimeToSecond(int hour, int min, int sec);

    void calculateTimeFromMinute(int time, int &hour, int &minute);
    int calculateTimeToMinute(int hour, int min);
    void stopStart(int start, int finish, int &play, int &stop, int &work, int &pause);
    void calculateAutoSwitch(int begin, int finish, int &start, int &stop, int &work, int &pause);
};

#endif