#ifndef RTC_EXTENTION_H
#define RTC_EXTENTION_H

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

class Date //: public DateTime
{
private:
    /* data */
public:
    Date(/* args */);
    ~Date();

    Date(const Date &date);
    Date(uint16_t year, uint8_t month, uint8_t day);

    uint16_t year() const { return 2000 + yOff; }
    uint8_t month() const { return m; }
    uint8_t day() const { return d; }
    uint8_t dayOfTheWeek() const;

protected:
    uint8_t yOff; ///< Year offset from 2000
    uint8_t m;    ///< Month 1-12
    uint8_t d;    ///< Day 1-31
};

class Time //: public DateTime
{
private:
    /* data */
public:
    Time(/* args */);
    ~Time();

    Time(const Time &time);
    Time(uint8_t hour = 0, uint8_t min = 0, uint8_t sec = 0);

    uint8_t hour() const { return hh; }
    uint8_t minute() const { return mm; }
    uint8_t second() const { return ss; }

protected:
    uint8_t hh; ///< Hours 0-23
    uint8_t mm; ///< Minutes 0-59
    uint8_t ss; ///< Seconds 0-59
};

class RTC_extention : public RTC_DS3231
{
private:
    /* data */
public:
    RTC_extention(/* args */);
    ~RTC_extention();

    static void adjustDate(const Date &dt);
    static void adjustTime(const Time &dt);

    static Date date();
    static Time time();
};

#endif