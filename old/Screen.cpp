#include "Screen.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Screen::Screen(/* args */) : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
{
}

Screen::~Screen()
{
}

void Screen::iGorLogo()
{
    setTextColor(1);
    setTextSize(1);

    setCursor(65, 20);
    print("Grow Light");
    setCursor(70, 30);
    print("Control");
    setCursor(65, 40);
    print("iGor_2019");
}

void Screen::showDigit(int value)
{
    if (value < 10)
    {
        print("0");
    }
    print(value);
    // value < 10 ? print("0") : print(value);
}

boolean Screen::blinkReady()
{
    boolean blink;

    if (millis() - prewCursorMillis >= blinkMillis)
    {
        blink = true;
    }

    else if (millis() - prewCursorMillis >= blinkMillis * 2)
    {
        prewCursorMillis = millis();
        blink = false;
    }

    return blink;
}

void Screen::showBlink(int value)
{
    if (blinkReady())
    {
        print("__");
    }

    else
    {
        showDigit(value);
    }
}

void Screen::showBlinkStringTime(int hh, int mm)
{

    if (cursor == (0 || 2))
    {
        cursorPosition = 0;
    }

    if (cursor == (1 || 3))
    {
        cursorPosition = 1;
    }

    if (cursor > 1)
    {
        choise = 1;
    }

    switch (cursorPosition)
    {
    case 0:
        showBlink(hh);
        print(":");
        showDigit(mm);
        break;
    case 1:
        showDigit(hh);
        print(":");
        showBlink(mm);
        break;

    default:
        break;
    }
}

void Screen::showStringTime(int hh, int mm)
{
    showDigit(hh);

    print(":");

    showDigit(mm);
}

void Screen::showStringFanTime(Watch &watch)
{
    showStringTime(watch.startHour, watch.startMin);

    print("-");

    showStringTime(watch.finishHour, watch.finishMin);
}

void Screen::showFanTime(Watch &watch, Key &key)
{
    if (key.dayReduration)
    {
        switch (choise)
        {
        case 0:
            showBlinkStringTime(watch.startHour, watch.startMin);
            print("-");
            showStringTime(watch.finishHour, watch.finishMin);
            break;

        case 1:
            showStringTime(watch.startHour, watch.startMin);
            print("-");
            showBlinkStringTime(watch.startHour, watch.startMin);
            break;

        default:
            showStringFanTime(watch);
            break;
        }
    }
    else
    {
        showStringFanTime(watch);
    }
}

void Screen::showAlert()
{
    print("manual mode");
}

void Screen::bottomLine(Watch &watch, Key &key)
{

    if (key.mode == key.MANUAL)
    {
        setCursor(0, 50);
        // setFont(u8g2_font_pixelmordred_tf);
        setTextSize(2);
        showAlert();
    }
    else
    {
        setCursor(5, 57);
        // setFont(u8g2_font_pressstart2p_8f);
        setTextSize(1);
        print(set[watch.onlyDay]);
        print(": ");

        // setCursor(30, 57);
        // setFont(u8g2_font_bitcasual_tn);
        setTextSize(1);
        showFanTime(watch, key);
    }
}

void Screen::showFanState(Switchers &relayState, Key &key)
{

    // setFont(u8g2_font_courB18_tr);
    setCursor(0, 24);
    setTextSize(3);
    print("FAN");

    // setFont(u8g2_font_HelvetiPixelOutline_tr);
    setCursor(30, 24);

    if (relayState.relaySW[0] || relayState.relaySW[1])
    {
        print(fanState[1]);
        setTextSize(1);
        // setFont(u8g2_font_pressstart2p_8f);

        if (relayState.relaySW[0])
        {
            setCursor(74, 24);
            print(fanState[2]);
        }
        else if (relayState.relaySW[1])
        {
            setCursor(74, 24);
            print(fanState[3]);
        }
    }
    else
    {
        print(fanState[0]);
    }
}

void Screen::showStringNowTime(int hh, int mm, int ss)
{
    showDigit(hh);

    print(":");

    showDigit(mm);

    print(":");

    showDigit(ss);
}

void Screen::highLighter(Key &key)
{
    switch (key.mode)
    {
    case key.LIGHT:

        // setFontMode(1);  /* activate transparent font mode */
        // setDrawColor(1); /* color 1 for the box */
        switch (key.highLight)
        {
        case key.DATE:
            // drawBox(0, 0, 70, 22);
            break;

        case key.TIME:
            // drawBox(70, 0, 58, 14);
            break;

        case key.START:
            // drawBox(5, 57, 30, 14);
            break;

        case key.STOP:
            // drawBox(30, 57, 30, 14);
            break;

        default:
            break;
        }

        // setDrawColor(2);
        break;

    default:
        break;
    }
}

void Screen::headerTime(Watch &watch)
{
    Time now = watch.time();

    // setFont(u8g2_font_courB08_tn);
    setTextSize(1);
    setCursor(80, 0);
    showStringNowTime(now.hour(), now.minute(), now.second());

    // setDrawColor(1);
}

void Screen::headerDate(Watch &watch)
{
    Date now = watch.date();

    // setFont(u8g2_font_courB08_tr);
    setTextSize(1);
    setCursor(0, 0);
    print(daysOfTheWeek[now.dayOfTheWeek()]);

    setCursor(0, 9);
    showDigit(now.day());
    print('/');
    showDigit(now.month());
    print('/');
    print(now.year());
}

void Screen::showHeaderScreen(Watch &watch)
{
    headerDate(watch);
    headerTime(watch);
}

void Screen::fanScreen(Switchers &relayState, Key &key, Watch &watch)
{
    // firstPage();
    // do
    // {
    clearDisplay();
    setTextColor(1);
    // highLighter(key);
    showHeaderScreen(watch);
    showFanState(relayState, key);
    bottomLine(watch, key);
    display();
    // } while (nextPage());
}