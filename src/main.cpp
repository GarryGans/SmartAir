#include <Arduino.h>
#include <Watch.h>
#include <Switchers.h>
#include <Screen.h>
#include <Key.h>

#define FAN_L1 7
#define FAN_L2 8

#define KB1x4 0

#define timeHoldKey 1000

byte speedPins[] = {FAN_L1, FAN_L2};
byte keyPin[] = {2, 3, 4, 5, 6};



Key key(keyPin);
Switchers switchers;
Watch watch;
Screen screen;

void setup()
{
  Serial.begin(9600);

  screen.begin();
  delay(100);

  watch.begin();
  watch.adjustTime(Time(23,57,55));
  // watch.adjustDate(Date(2018, 6, 8));
  // watch.adjust(DateTime(F(__DATE__), F(__TIME__)));

  delay(100);

  key.begin(KB1x4, timeHoldKey);
  delay(100);

  switchers.begin(speedPins);
  delay(100);
}

void loop()
{
  key.keyCommands();
  watch.autoFlow(key);
  switchers.switcher(key, watch);
  screen.fanScreen(switchers, key, watch);
}