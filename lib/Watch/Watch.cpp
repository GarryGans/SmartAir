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

void Watch::correctStop(int finishDay, int &play, int &stop)
{
    if (play < finishDay)
    {
        if (stop > finishDay && play < stop)
        {
            stop = finishDay;
        }

        else if (play > stop && stop < finishDay)
        {
            stop = finishDay;
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

void Watch::stopStart(int startDay, int finishDay, int &play, int &stop, int &work, int &pause)
{
    if (newDay && !night)
    {
        newDay = false;
    }

    if (nowTime() >= stop && !newDay)
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
            if (startDay < finishDay)
            {
                if (play >= finishDay)
                {
                    work = nightWork;
                }
            }
            else if (startDay > finishDay && play >= midNightAfter)
            {
                if (play >= finishDay)
                {
                    work = nightWork;
                }
            }
        }

        stop = play + work;
        midNigth(stop);

        if (!night)
        {
            if (startDay < finishDay)
            {
                correctStop(finishDay, play, stop);
            }

            else if (startDay > finishDay && play >= midNightAfter)
            {
                correctStop(finishDay, play, stop);
            }
        }

        stopFog = stop + fogTime;
        midNigth(stopFog);

        timeFromMinute(play, playHour, playMin);
        timeFromMinute(stop, stopHour, stophMin);
    }

    if (onlyDay && night)
    {
        newDay = true;

        play = startDay;
        stop = play + work;

        timeFromMinute(play, playHour, playMin);
        timeFromMinute(stop, stopHour, stophMin);
    }
}

void Watch::setDuration(int startDay, int finishDay, int &work, int &pause)
{
    if (startDay == finishDay)
    {
        night = false;
    }

    else if (startDay < finishDay)
    {
        if ((nowTime() >= startDay && nowTime() < finishDay))
        {
            night = false;
        }

        else
        {
            night = true;
        }
    }

    else if (startDay > finishDay)
    {
        if ((nowTime() >= startDay && nowTime() < midNightBefore) || (nowTime() >= midNightAfter && nowTime() < finishDay))
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

void Watch::calculateAutoSwitch(int startDay, int finishDay, int &play, int &stop, int &work, int &pause)
{
    setDuration(startDay, finishDay, work, pause);

    stopStart(startDay, finishDay, play, stop, work, pause);
}

void Watch::autoFlow(Key &key)
{
    if (key.mode != key.MANUAL)
    {
        startDay = timeToMinute(startHour, startMin);
        finishDay = timeToMinute(finishHour, finishMin);

        calculateAutoSwitch(startDay, finishDay, play, stop, work, pause);

        switchFlow(play, stop, autoSwitch);
    }
}

void Watch::autoFog(Key &key)
{
    if (key.mode != key.MANUAL)
    {
        if (fog)
        {
            if (nowTime() < stopFog)
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
        }
    }
}
