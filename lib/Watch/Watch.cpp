#include "Watch.h"

Watch::Watch() : RTC_extention()
{
}

Watch::~Watch()
{
}

void Watch::calculateTimeFromMinute(int time, int &hour, int &minute)
{
    hour = 0;
    minute = 0;

    if (time < 60)
    {
        hour = 0;
    }
    else
    {
        while (time >= 60)
        {
            time -= 60;
            hour++;
        }
    }

    minute = time;
}

int Watch::calculateTimeToSecond(int hour, int min, int sec)
{
    return ((hour * 60 + min) * 60 + sec);
}

int Watch::calculateTimeToMinute(int hour, int min)
{
    return (hour * 60 + min);
}

int Watch::nowSec()
{
    Time now = time();
    return (calculateTimeToSecond(now.hour(), now.minute(), now.second()));
}

int Watch::nowTime()
{
    Time now = time();
    return (calculateTimeToMinute(now.hour(), now.minute()));
}

void Watch::stopStart(int start, int finish, int &play, int &stop, int &work, int &pause)
{
    if (!firstStart)
    {
        play = nowTime();
        firstStart = true;
    }

    else
    {
        play = stop + pause;
        if (play > midNightBefore)
        {
            play -= 24 * 60;
        }
    }

    if (!onlyDay && play >= finish)
    {
        work = nightWork;
    }

    stop = play + work;

    if (stop > midNightBefore)
    {
        stop -= 24 * 60;
    }

    if (play > stop)
    {
        if (onlyDay && stop < finish && finish <= midNightBefore)
        {
            stop = finish;
        }
        else if (onlyDay && stop > finish)
        {
            stop = finish;
        }
    }
    else
    {
        if (onlyDay && stop > finish)
        {
            stop = finish;
        }
    }

    calculateTimeFromMinute(play, playHour, playMin);
    calculateTimeFromMinute(stop, stopHour, stophMin);

    Serial.print("play ");
    Serial.println(play);
    Serial.print("stop ");
    Serial.println(stop);
}

void Watch::calculateAutoSwitch(int start, int finish, int &play, int &stop, int &work, int &pause)
{
    if (play > stop)
    {
        if (nowTime() >= stop && nowTime() < play - pause)
        {
            stopStart(start, finish, play, stop, work, pause);
        }
    }
    else
    {
        if (nowTime() >= stop)
        {
            stopStart(start, finish, play, stop, work, pause);
        }
    }
}

void Watch::switchFlow(int play, int stop, boolean autoSwitch[])
{
    if (play > stop)
    {
        if ((nowTime() >= play && nowTime() <= midNightBefore) || (nowTime() >= midNightAfter && nowTime() < stop))
        {
            autoSwitch[flowPin] = true;
        }
        else
        {
            autoSwitch[flowPin] = false;
        }
    }

    else if (play < stop)
    {
        if (nowTime() >= play && nowTime() < stop)
        {
            autoSwitch[flowPin] = true;
        }
        else
        {
            autoSwitch[flowPin] = false;
        }
    }
    else
    {
        for (byte i = 0; i < speedPinsAmount; i++)
        {
            autoSwitch[i] = false;
        }
    }

    // if (nowTime() >= stop && nowTime() < play)
    // {
    //     autoSwitch[flowPin] = false;
    // }
    // else
    // {
    //     autoSwitch[flowPin] = true;
    // }
}

void Watch::autoFlow(Key &key)
{
    if (key.mode != key.MANUAL)
    {
        start = calculateTimeToMinute(startHour, startMin);
        finish = calculateTimeToMinute(finishHour, finishMin);

        if ((nowTime() >= start && nowTime() < finish) || onlyDay || start == finish)
        {
            work = dayWork;
            pause = dayPause;
        }
        else
        {
            work = nightWork;
            pause = nightPause;
        }

        if (start == finish)
        {
            onlyDay = false;
            calculateAutoSwitch(start, finish, play, stop, work, pause);
        }

        else if (start > finish)
        {
            if (onlyDay)
            {
                if ((nowTime() >= start && nowTime() <= midNightBefore) || (nowTime() >= midNightAfter && nowTime() < finish))
                {
                    calculateAutoSwitch(start, finish, play, stop, work, pause);
                }
            }
            else if (!onlyDay)
            {
                calculateAutoSwitch(start, finish, play, stop, work, pause);
            }
        }

        else if (start < finish)
        {
            if (onlyDay && nowTime() >= start && nowTime() < finish)
            {
                calculateAutoSwitch(start, finish, play, stop, work, pause);
            }
            else if (!onlyDay)
            {
                calculateAutoSwitch(start, finish, play, stop, work, pause);
            }
        }

        

        switchFlow(play, stop, autoSwitch);

        if (onlyDay && !autoSwitch[flowPin] && stop == finish)
        {
            play = start;
            stop = play + work;
            firstStart = false;
            calculateTimeFromMinute(play, playHour, playMin);
            calculateTimeFromMinute(stop, stopHour, stophMin);
            Serial.println("firstStart");
            Serial.print("play ");
            Serial.println(play);
            Serial.print("stop ");
            Serial.println(stop);
        }
    }
}
