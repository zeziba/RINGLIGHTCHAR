#include <Arduino.h>

#include "env.h"
#include "pins.h"
#include "neo_pixel_effects.h"
#include "sserver.h"
#include <TaskScheduler.h>

Scheduler runner;
void callback1();

Task pixelTask1(DELAYVAL * 1000, TASK_FOREVER, &callback1, &runner, true);

void setup()
{
#ifdef SERIAL_ENABLE
  Serial.begin(BAUD_RATE);
  Serial.println();
#endif

  wifiServerStart();

  neoPixelStart();
}

void loop()
{
  runner.execute();
  server.handleClient();
}

void callback1()
{
  neoPixelEffects(0);
}
