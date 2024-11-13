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

#endif
