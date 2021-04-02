#include "Watch.h"

Watch::Watch() : RTC_extention()
{
}

Watch::~Watch()
{
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

void Watch::calculateAutoSwitch(int start, int finish, int &play, int &stop, int &work, int &pause)
{
    if (play > stop)
    {
        if (nowTime() >= stop && nowTime() < play - pause)
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

            if (play >= finish && nowTime() < finish)
            {
                work = nightWork;
            }

            stop = play + work;

            if (stop > midNightBefore)
            {
                stop -= 24 * 60;
            }

            if (onlyDay && stop >= finish && nowTime() < finish)
            {
                stop = finish;
            }

            Serial.print("play ");
            Serial.println(play);
            Serial.print("stop ");
            Serial.println(stop);
        }
    }
    else
    {
        if (nowTime() >= stop)
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

            if (play >= finish && nowTime() < finish)
            {
                work = nightWork;
            }

            stop = play + work;

            if (stop > midNightBefore)
            {
                stop -= 24 * 60;
            }

            if (onlyDay && stop >= finish && nowTime() < finish)
            {
                stop = finish;
            }

            Serial.print("play ");
            Serial.println(play);
            Serial.print("stop ");
            Serial.println(stop);
        }
    }
    
    
}

void Watch::switchFlow(int play, int stop, boolean autoSwitch[])
{
    if (play > stop)
    {
        if ((nowTime() >= play && nowTime() <= midNightBefore) || (nowTime() >= midNightAfter && nowTime() < stop))
        {
            autoSwitch[0] = true;
        }
        else
        {
            for (byte i = 0; i < speedPinsAmount; i++)
            {
                autoSwitch[i] = false;
            }
        }
    }

    else if (play < stop)
    {
        if (nowTime() >= play && nowTime() < stop)
        {
            autoSwitch[0] = true;
        }
        else
        {
            for (byte i = 0; i < speedPinsAmount; i++)
            {
                autoSwitch[i] = false;
            }
        }
    }
    else
    {
        for (byte i = 0; i < speedPinsAmount; i++)
        {
            autoSwitch[i] = false;
        }
    }
}

void Watch::autoFlow(Key &key)
{
    if (key.mode != key.MANUAL)
    {
        start = calculateTimeToMinute(startHour, startMin);
        finish = calculateTimeToMinute(finishHour, finishMin);

        if ((nowTime() >= start && nowTime() < finish) || onlyDay)
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

        // if (onlyDay && !autoSwitch[0] && stop == finish)
        // {
        //     firstStart = false;
        // }
    }
}
