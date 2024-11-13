#include <Adafruit_NeoPixel.h>
#include "Adafruit_TestBed.h"

extern Adafruit_TestBed TB;

void ledInit() {
  TB.neopixelPin = PIN_NEOPIXEL;
  TB.neopixelNum = 1; 
  TB.begin();
}

void ledOn() {
  TB.setColor(WHITE);
}
