#include <Arduino.h>
#include <Watch.h>
#include <Switchers.h>
#include <Screen.h>
#include <Key.h>

#define FLOW_L1 7
#define FLOW_L2 8
#define FAN 9
#define FOG_L 10
#define FOG_But 11

#define KB1x4 0

#define timeHoldKey 1000

byte speedPins[] = {FLOW_L1, FLOW_L2};
byte fanPin = FAN;
byte fogPin = FOG_L;
byte fogBut = FOG_But;
byte keyPins[] = {2, 3, 4, 5, 6};

Key key(keyPins);
Switchers switchers;
Watch watch;
Screen screen;

void setup()
{
  // Serial.begin(9600);

  screen.begin();
  delay(100);

  watch.begin();
  // tes
  //  watch.adjust(DateTime(F(__DATE__), F(__TIME__)));
  delay(100);

  key.begin(KB1x4, timeHoldKey);
  delay(100);

  switchers.begin(speedPins, fanPin, fogPin, fogBut);
  delay(100);
}

void loop()
{
  // key.keyCommands();
  watch.autoFlow(key);
  switchers.flowSwitcher(key, watch);

  watch.autoFan(key);
  switchers.fanSwitcher(key, watch);

  watch.autoFog(key);
  switchers.fogSwitcher(key, watch);

  screen.fanScreen(switchers, key, watch);
}