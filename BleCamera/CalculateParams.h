#ifndef CALCULATE_PARAMS_H
#define CALCULATE_PARAMS_H

#include <cstdint> // Include this line

// 曝光模式枚举
enum ExposureMode {
  AUTO,           // 自动曝光
  APERTURE,       // 光圈优先
  SHUTTER         // 快门优先
};

void calculateInit();
uint16_t readLUX();
void calculate();
// double calculate_ev(double aperture, double shutter_speed, double iso);
// double calculate_lux(double aperture, double shutter_speed, double iso);
// double calculate_ev_from_lux(double lux);
// double calculate_iso(double aperture, double shutter_speed, double lux);

#endif
