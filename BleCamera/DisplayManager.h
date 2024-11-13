#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "CalculateParams.h"

void tftInit();
void flashScreen();
void displayParameters(int aperture, int shutter_speed, int lux, int iso, ExposureMode exposureMode);

#endif
