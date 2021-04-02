#include "RTC_extention.h"

#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#elif defined(ARDUINO_ARCH_SAMD)
// nothing special needed
#elif defined(ARDUINO_SAM_DUE)
#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define Wire Wire1
#endif

#define _I2C_WRITE write ///< Modern I2C write
#define _I2C_READ read   ///< Modern I2C read



/*!
    Read a byte from an I2C register
    param addr I2C address
    param reg Register address
    return Register value
*/

static uint8_t read_i2c_register(uint8_t addr, uint8_t reg)
{
    Wire.beginTransmission(addr);
    Wire._I2C_WRITE((byte)reg);
    Wire.endTransmission();

    Wire.requestFrom(addr, (byte)1);
    return Wire._I2C_READ();
}

/*!
    Write a byte to an I2C register
    param addr I2C address
    param reg Register address
    param val Value to write
*/
static void write_i2c_register(uint8_t addr, uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(addr);
    Wire._I2C_WRITE((byte)reg);
    Wire._I2C_WRITE((byte)val);
    Wire.endTransmission();
}

/**
  Number of days in each month, from January to November. December is not
  needed. Omitting it avoids an incompatibility with Paul Stoffregen's Time
  library. C.f. https://github.com/adafruit/RTClib/issues/114
*/

const uint8_t daysInMonth[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

/*!
    Given a date, return number of days since 2000/01/01, valid for 2001..2099
    param y Year
    param m Month
    param d Day
    return Number of days
*/

static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d)
{
    if (y >= 2000)
        y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
        days += pgm_read_byte(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0)
        ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}

/*!
    Convert a binary value to BCD format for the RTC registers
    param val Binary value
    return BCD value
*/

static uint8_t bin2bcd(uint8_t val) { return val + 6 * (val / 10); }

/*!
    Convert a binary coded decimal value to binary. RTC stores time/date values as BCD.
    param val BCD value
    return Binary value
*/
static uint8_t bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }

/*******
 * 
 */



Date::Date(/* args */)
{
}

Date::~Date()
{
}

Time::Time(/* args */)
{
}

Time::~Time()
{
}

RTC_extention::RTC_extention(/* args */) : RTC_DS3231()
{
}

RTC_extention::~RTC_extention()
{
}

Date::Date(uint16_t year, uint8_t month, uint8_t day)
{
    if (year >= 2000)
        year -= 2000;
    yOff = year;
    m = month;
    d = day;
}

Date::Date(const Date &date) : yOff(date.yOff),
                               m(date.m),
                               d(date.d)
{
}

Time::Time(uint8_t hour, uint8_t min, uint8_t sec)
{
    hh = hour;
    mm = min;
    ss = sec;
}

Time::Time(const Time &time) : hh(time.hh),
                               mm(time.mm),
                               ss(time.ss)
{
}

uint8_t Date::dayOfTheWeek() const
{
    uint16_t day = date2days(yOff, m, d);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

void RTC_extention::adjustDate(const Date &dt)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire._I2C_WRITE((byte)3); // start at location 3
    Wire._I2C_WRITE(bin2bcd(0));
    Wire._I2C_WRITE(bin2bcd(dt.day()));
    Wire._I2C_WRITE(bin2bcd(dt.month()));
    Wire._I2C_WRITE(bin2bcd(dt.year() - 2000));
    Wire.endTransmission();

    uint8_t statreg = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
    statreg &= ~0x80; // flip OSF bit
    write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, statreg);
}

void RTC_extention::adjustTime(const Time &dt)
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire._I2C_WRITE((byte)0); // start at location 0
    Wire._I2C_WRITE(bin2bcd(dt.second()));
    Wire._I2C_WRITE(bin2bcd(dt.minute()));
    Wire._I2C_WRITE(bin2bcd(dt.hour()));
    Wire.endTransmission();

    uint8_t statreg = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
    statreg &= ~0x80; // flip OSF bit
    write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, statreg);
}

Date RTC_extention::date()
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire._I2C_WRITE((byte)4); // start at location 4
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDRESS, 7);

    uint8_t d = bcd2bin(Wire._I2C_READ());
    uint8_t m = bcd2bin(Wire._I2C_READ());
    uint16_t y = bcd2bin(Wire._I2C_READ()) + 2000;

    return Date(y, m, d);
}

Time RTC_extention::time()
{
    Wire.beginTransmission(DS3231_ADDRESS);
    Wire._I2C_WRITE((byte)0); // start at location 0
    Wire.endTransmission();

    Wire.requestFrom(DS3231_ADDRESS, 7);

    uint8_t ss = bcd2bin(Wire._I2C_READ() & 0x7F);
    uint8_t mm = bcd2bin(Wire._I2C_READ());
    uint8_t hh = bcd2bin(Wire._I2C_READ());

    return Time(hh, mm, ss);
}
