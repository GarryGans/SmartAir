#include "Watch.h"

Watch::Watch() : RTC_extention()
{
}

Watch::~Watch()
{
}

void Watch::timeFromMinute(int time, int &hour, int &minute)
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

int Watch::timeToSecond(int hour, int min, int sec)
{
    return ((hour * 60 + min) * 60 + sec);
}

int Watch::timeToMinute(int hour, int min)
{
    return (hour * 60 + min);
}

int Watch::nowSec()
{
    Time now = time();
    return (timeToSecond(now.hour(), now.minute(), now.second()));
}

int Watch::nowTime()
{
    Time now = time();
    return (timeToMinute(now.hour(), now.minute()));
}

void Watch::switchFlow(int play, int stop, boolean autoSwitch[])
{
    if (play < stop)
    {
        if (nowTime() >= play && nowTime() < stop)
        {
            autoSwitch[flowPin] = true;
            fog = false;
        }
        else
        {
            autoSwitch[flowPin] = false;
            fog = true;
        }
    }

    else if (play > stop)
    {
        if ((nowTime() >= play && nowTime() <= midNightBefore) || (nowTime() >= midNightAfter && nowTime() < stop))
        {
            autoSwitch[flowPin] = true;
            fog = false;
        }
        else
        {
            autoSwitch[flowPin] = false;
            fog = true;
        }
    }

    else
    {
        for (byte i = 0; i < speedPinsAmount; i++)
        {
            autoSwitch[i] = false;
            fog = false;
        }
    }
}

void Watch::correctStop(int finish, int &play, int &stop)
{
    if (play < finish)
    {
        if (play < stop)
        {
            if (stop > finish)
            {
                stop = finish;
            }
        }
        else if (play > stop)
        {
            if (stop < finish)
            {
                stop = finish;
            }
        }
    }
}

void Watch::midNigth(int &value)
{
    if (value > midNightBefore)
    {
        value -= 24 * 60;
    }
}

void Watch::stopStart(int start, int finish, int &play, int &stop, int &work, int &pause)
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
            midNigth(play);
        }

        if (!onlyDay)
        {
            if (start < finish)
            {
                if (play >= finish)
                {
                    work = nightWork;
                }
            }
            else if (start > finish && play >= midNightAfter)
            {
                if (play >= finish)
                {
                    work = nightWork;
                }
            }
        }

        stop = play + work;
        midNigth(stop);

        if (!night)
        {
            if (start < finish)
            {
                correctStop(finish, play, stop);
            }

            else if (start > finish && play >= midNightAfter)
            {
                correctStop(finish, play, stop);
            }
        }

        stopFog = stop + fogTime;
        midNigth(stopFog);

        timeFromMinute(play, playHour, playMin);
        timeFromMinute(stop, stopHour, stophMin);
    }
}

void Watch::setDuration(int start, int finish, int &work, int &pause)
{
    if (onlyDay || start == finish)
    {
        night = false;
    }

    else if (start < finish)
    {
        if ((nowTime() >= start && nowTime() < finish))
        {
            night = false;
        }

        else
        {
            night = true;
        }
    }

    else if (start > finish)
    {
        if ((nowTime() >= start && nowTime() < midNightBefore) || (nowTime() >= midNightAfter && nowTime() < finish))
        {
            night = false;
        }

        else
        {
            night = true;
        }
    }

    if (!night)
    {
        work = dayWork;
        pause = dayPause;
    }

    else
    {
        work = nightWork;
        pause = nightPause;
    }
}

void Watch::calculateAutoSwitch(int start, int finish, int &play, int &stop, int &work, int &pause)
{
    setDuration(start, finish, work, pause);

    stopStart(start, finish, play, stop, work, pause);
}

void Watch::autoFlow(Key &key)
{
    if (key.mode != key.MANUAL)
    {
        start = timeToMinute(startHour, startMin);
        finish = timeToMinute(finishHour, finishMin);

        if (start == finish)
        {
            calculateAutoSwitch(start, finish, play, stop, work, pause);
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

        switchFlow(play, stop, autoSwitch);

        // need to EDIT
        if (!(start == finish))
        {
            if (onlyDay && !autoSwitch[flowPin] && stop >= finish)
            {
                night = true;

                play = start;
                stop = play + work;

                firstStart = false;

                timeFromMinute(play, playHour, playMin);
                timeFromMinute(stop, stopHour, stophMin);

                // Serial.println("firstStart");
                // Serial.print("play ");
                // Serial.println(play);
                // Serial.print("stop ");
                // Serial.println(stop);
            }
        }
        //end
    }
}

void Watch::autoFog(Key &key)
{
    if (key.mode != key.MANUAL)
    {
        if (fog)
        {
            if (nowTime() > stop && nowTime() < stopFog)
            {
                fogSwitch = true;
            }
            else
            {
                fogSwitch = false;
            }

            if (fogSwitch)
            {
                fogBut = true;
            }
            else
            {
                fogBut = false;
            }

            fogBut = true;
        }
    }
}
