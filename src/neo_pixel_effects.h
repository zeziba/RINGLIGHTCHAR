#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define NUMPIXELS 16

// Get NeoPixel ready to be used
static Adafruit_NeoPixel pixels(NUMPIXELS, D5, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 10

// Values for RGB colors
int r = 0, g = 0, b = 0;
int maxCount = 255, divCount = 48;

static void neoPixelStart()
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
}

void effect0()
{
  pixels.clear();

  for (int i = 0; i < NUMPIXELS; i++)
  {

    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();

    b += maxCount / divCount;
  }

  if (b > maxCount)
  {
    b = 0;
    g += maxCount / divCount;
  }
  if (g > maxCount)
  {
    g = 0;
    r += maxCount / divCount;
  }
  if (r > maxCount)
    r = 0;
}

static void neoPixelEffects(int effectNum)
{
  switch (effectNum)
  {
  case 0:
    effect0();
    break;

  default:
    break;
  }
}