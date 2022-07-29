#include <Arduino.h>

#include "env.h"
#include "pins.h"
#include "neo_pixel_effects.h"
#include "sserver.h"

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
}
