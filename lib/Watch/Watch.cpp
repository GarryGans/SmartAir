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

byte Watch::setFlowPin()
{
    if (nowTime() >= morning && nowTime() < evening)
    {
        flowPin = 1;

        // if (nowTime() >= morning && nowTime() < morning + hour)
        // {
        //     flowPin = 1;
        // }
        // else if (nowTime() >= morning + 2 * hour && nowTime() < morning + 3 * hour)
        // {
        //     flowPin = 1;
        // }
        // else if (nowTime() >= morning + 4 * hour && nowTime() < morning + 5 * hour)
        // {
        //     flowPin = 1;
        // }
        // else if (nowTime() >= morning + 6 * hour && nowTime() < morning + 7 * hour)
        // {
        //     flowPin = 1;
        // }
        // else if (nowTime() >= morning + 8 * hour && nowTime() < morning + 9 * hour)
        // {
        //     flowPin = 1;
        // }
        // else if (nowTime() >= morning + 10 * hour && nowTime() < morning + 11 * hour)
        // {
        //     flowPin = 1;
        // }
    }

    else
    {
        flowPin = 0;
    }

    if (flowPin == 0)
    {
        flowSwitch[1] = false;
    }
    else if (flowPin == 1)
    {
        flowSwitch[0] = false;
    }

    return flowPin;
}

void Watch::switchFlow(int play, int stop, boolean flowSwitch[])
{
    if (play < stop)
    {
        if (nowTime() >= play && nowTime() < stop)
        {
            flowSwitch[setFlowPin()] = true;
            fog = false;
        }
        else
        {
            for (byte i = 0; i < speedPinsAmount; i++)
            {
                flowSwitch[i] = false;
                fog = true;
            }
        }
    }

    else if (play > stop)
    {
        if ((nowTime() >= play && nowTime() <= midNightBefore) || (nowTime() >= midNightAfter && nowTime() < stop))
        {
            flowSwitch[setFlowPin()] = true;
            fog = false;
        }
        else
        {
            for (byte i = 0; i < speedPinsAmount; i++)
            {
                flowSwitch[i] = false;
                fog = true;
            }
        }
    }

    else
    {
        for (byte i = 0; i < speedPinsAmount; i++)
        {
            flowSwitch[i] = false;
            fog = false;
        }
    }

    if (nowTime() == stop && newDuration)
    {
        newDuration = false;
    }
}

void Watch::midNigth(int &value)
{
    if (value > midNightBefore)
    {
        value -= 24 * 60;
    }
}

void Watch::correctWork(int startDay, int finishDay, int play, int &work)
{
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

    if (night && play >= startDay && nowTime() < startDay)
    {
        work = dayWork;
    }
}

void Watch::calculateStop(int startDay, int finishDay, int play, int &stop, int &work, int pause)
{
    correctWork(startDay, finishDay, play, work);

    if (pause == 0)
    {
        if ((onlyDay && night) || !night)
        {
            stop = finishDay;
        }
        else if (night)
        {
            stop = startDay;
        }
    }
    else
    {
        stop = play + work;
        midNigth(stop);

        if (!night)
        {
            if (startDay < finishDay)
            {
                stop = constrain(stop, startDay, finishDay);
            }

            else if (startDay > finishDay && stop <= startDay)
            {
                stop = constrain(stop, midNightAfter, finishDay);
            }
        }

        else if (night)
        {
            if (startDay < finishDay && stop <= startDay)
            {
                stop = constrain(stop, midNightAfter, startDay);
            }

            else if (startDay > finishDay)
            {
                stop = constrain(stop, finishDay, startDay);
            }
        }
    }
}

void Watch::calculatePlay(int startDay, int &play, int stop, int pause)
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
        play = stop + pause;
        midNigth(play);

        if (night && stop <= startDay && play > startDay)
        {
            play = startDay;
        }
    }
}

void Watch::stopStart(int startDay, int finishDay, int &play, int &stop, int &work, int pause)
{
    if (!newDuration || !firstStart)
    {
        calculatePlay(startDay, play, stop, pause);
        calculateStop(startDay, finishDay, play, stop, work, pause);
        newDuration = true;

        timeFromMinute(play, playHour, playMin);
        timeFromMinute(stop, stopHour, stophMin);

        // if (!fogSwitch)
        // {
        //     stopFog = stop + fogTime;
        //     midNigth(stopFog);
        // }

        // Serial.print("stopFog ");
        // int h;
        // int m;
        // timeFromMinute(stopFog, h, m);
        // Serial.print(h);
        // Serial.print(":");
        // Serial.println(m);
    }
}

void Watch::setDayNight(int startDay, int finishDay, int &work, int &pause)
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

void Watch::calculateFlowSwitch(int startDay, int finishDay, int &play, int &stop, int &work, int &pause)
{
    setDayNight(startDay, finishDay, work, pause);

    stopStart(startDay, finishDay, play, stop, work, pause);
}

void Watch::autoFlow(Key &key)
{
    if (key.mode != key.MANUAL)
    {
        startDay = timeToMinute(startHour, startMin);
        finishDay = timeToMinute(finishHour, finishMin);

        calculateFlowSwitch(startDay, finishDay, play, stop, work, pause);

        switchFlow(play, stop, flowSwitch);
    }
}

void Watch::autoFan(Key &key)
{
    if (key.mode != key.MANUAL)
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
            // if (fogBut)
            // {
            //     fogBut = false;
            // }
            else
            {
                fogBut = false;
            }
        }
    }
}
