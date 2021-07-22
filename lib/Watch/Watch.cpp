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

    while (time >= 60)
    {
        time -= 60;
        hour++;
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

byte Watch::speedPin()
{
    if (nowTime() >= morning && nowTime() < evening)
    {
        flowPin = 1;
        flowSwitch[0] = false;
    }

    else
    {
        flowPin = 0;
        flowSwitch[1] = false;
    }

    return flowPin;
}

void Watch::switchFlow()
{
    if (play < stop)
    {
        if (nowTime() >= play && nowTime() < stop)
        {
            flowSwitch[speedPin()] = true;
        }
        else
        {
            flowSwitch[speedPin()] = false;
        }
    }

    else if (play > stop)
    {
        if ((nowTime() >= play && nowTime() <= midNightBefore) || (nowTime() >= midNightAfter && nowTime() < stop))
        {
            flowSwitch[speedPin()] = true;
        }
        else
        {
            flowSwitch[speedPin()] = false;
        }
    }

    else
    {
        for (byte i = 0; i < speedPinsAmount; i++)
        {
            flowSwitch[i] = false;
        }
    }

    if (nowTime() == stop && newDuration)
    {
        newDuration = false;
    }
}

int Watch::midNigth(int value)
{
    if (value > midNightBefore)
    {
        value -= 24 * 60;
    }

    return value;
}

int Watch::next(int day, int work)
{
    return midNigth(day + work);
}

void Watch::correctStop(int &stop)
{
    if (onlyDay)
    {
        if (stop > finishDay && nowTime() < finishDay)
        {
            stop = finishDay;
        }
    }

    else
    {
        if (night)
        {
            if (stop > startDay && nowTime() < startDay)
            {
                stop = next(startDay, dayWork);
            }
        }

        else
        {
            if (stop > finishDay && nowTime() < finishDay)
            {
                stop = next(finishDay, nightWork);
            }
        }
    }
}

void Watch::calculateStop()
{
    if (pause == 0)
    {
        if ((onlyDay && night) || !night)
        {
            stop = finishDay;
        }
        else if (!onlyDay && night)
        {
            stop = startDay;
        }
    }
    else
    {
        stop = next(play, work);
        correctStop(stop);
    }
}

void Watch::correctWork()
{
    if (!onlyDay)
    {
        if (night)
        {
            if (play >= startDay && nowTime() < startDay)
            {
                work = dayWork;
                pause = dayPause;
            }
        }

        else
        {
            if (play >= finishDay && nowTime() < finishDay)
            {
                work = nightWork;
            }
        }
    }
}

void Watch::calculatePlay()
{
    if ((night && onlyDay) || (stop == startDay))
    {
        play = startDay;
    }

    else if (!firstStart)
    {
        play = nowTime();
        firstStart = true;
    }

    else
    {
        play = next(stop, pause);
    }

    correctWork();
}

void Watch::stopStart()
{
    if (!newDuration)
    {
        calculatePlay();
        timeFromMinute(play, playHour, playMin);

        calculateStop();
        timeFromMinute(stop, stopHour, stophMin);

        newDuration = true;
    }
}

void Watch::setDayNight()
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

    if (night && !onlyDay)
    {
        work = nightWork;
        pause = nightPause;
    }
    else
    {
        work = dayWork;
        pause = dayPause;
    }
}

void Watch::calculateFlowSwitch()
{
    setDayNight();

    stopStart();
}

void Watch::autoFlow(Key &key)
{
    if (key.mode != key.HAND)
    {
        startDay = timeToMinute(startHour, startMin);
        finishDay = timeToMinute(finishHour, finishMin);

        calculateFlowSwitch();

        switchFlow();
    }
}

void Watch::autoFan(Key &key)
{
    if (key.mode != key.HAND)
    {
        if (flowSwitch[flowPin])
        {
            fanSwitch = true;
        }
        else
        {
            fanSwitch = false;
        }
    }
}

void Watch::autoFog(Key &key)
{
    if (key.mode != key.HAND)
    {
        if (fog)
        {
            fogSwitch = true;
        }
        else
        {
            fogSwitch = false;
        }
    }
}
