#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDLER_H
#include "CalculateParams.h"

// ISO 范围限制
#define ISO_MIN 100
#define ISO_MAX 102400

// 快门速度范围
#define SHUTTER_MIN 20    // 最慢 1/20 秒
#define SHUTTER_MAX 6000  // 最快 1/6000 秒

#include <M5_DLight.h>

extern int currentAperture;      // 模拟光圈值，如 f/1.8
extern int currentShutterSpeed;  // 模拟快门速度，如 1/1000 秒
extern int currentISO;           // 模拟 ISO 值

M5_DLight sensor;
uint16_t lux = 0;
ExposureMode exposureMode = AUTO;

// 基础曝光参数结构体
struct ExposureParams {
  float aperture;  // 光圈值 F值
  int shutter;   // 快门速度(秒)
  int iso;         // ISO感光度
  float lux;       // 环境光照度(lux)
};

void calculateInit() {
  sensor.begin();

  // if (!sensor.begin()) {
  //   Serial.println("光照传感器初始化失败");
  //   while (1);
  // }
  // Serial.println("光照传感器初始化成功");
  sensor.setMode(CONTINUOUSLY_H_RESOLUTION_MODE);
}

uint16_t readLUX() {
  lux = sensor.getLUX();
  return lux;
}



// 自动曝光计算
ExposureParams calculateAutoExposure(float lux) {
  ExposureParams params;

  // 基于光照度计算合适的曝光参数
  if (lux > 100000) {
    params.aperture = 16.0;
    params.shutter = 1000;
    params.iso = 100;
  } else if (lux > 10000) {
    params.aperture = 8.0;
    params.shutter =  500;
    params.iso = 200;
  } else if (lux > 1000) {
    params.aperture = 5.6;
    params.shutter = 250;
    params.iso = 400;
  } else if (lux > 100) {
    params.aperture = 4.0;
    params.shutter =  125;
    params.iso = 600;
  } else if (lux > 10) {
    params.aperture = 2.8;
    params.shutter = 60;
    params.iso = 800;
  } else {
    params.aperture = 2.0;
    params.shutter = 30;
    params.iso = 1600;
  }
  params.lux = lux;
  return params;
}


// 光圈优先模式计算
ExposureParams calculateAperturePriority(float lux, float targetAperture) {
  ExposureParams params;
  params.aperture = targetAperture;

  // 基于目标光圈和光照度计算快门速度
  float ev = log2(lux / 2.5);
  params.shutter =  (pow(2, ev) / params.aperture * params.aperture);

  // 根据快门速度调整ISO
  if (params.shutter < 2000) {
    params.iso = 100;
  } else if (params.shutter < 500) {
    params.iso = 200;
  } else if (params.shutter < 125) {
    params.iso = 400;
  } else {
    params.iso = 800;
  }

  params.lux = lux;
  return params;
}

// 快门优先模式计算
ExposureParams calculateShutterPriority(float lux, float targetShutter) {
  ExposureParams params;
  params.shutter = targetShutter;

  // 基于目标快门速度和光照度计算光圈值
  float ev = log2(lux / 2.5);
  params.aperture = sqrt(pow(2, ev) * targetShutter);

  // 根据光圈值调整ISO
  if (params.aperture > 11) {
    params.iso = 100;
  } else if (params.aperture > 8) {
    params.iso = 200;
  } else if (params.aperture > 4) {
    params.iso = 400;
  } else {
    params.iso = 800;
  }

  params.lux = lux;
  return params;
}


ExposureParams calculateExposure(ExposureMode mode, float lux) {
  if (mode == AUTO) {
    return calculateAutoExposure(lux);
  } else if (mode == APERTURE) {
    return calculateAperturePriority(lux, currentAperture / 10.0f);
  } else {
    return calculateShutterPriority(lux, currentShutterSpeed);
  }
}

void calculate() {
  ExposureParams params = calculateExposure(exposureMode, lux);
  currentAperture = params.aperture * 10;
  currentShutterSpeed =  params.shutter;
  currentISO = params.iso;
  Serial.print("当前拍摄模式: ");
  Serial.println(exposureMode);

  Serial.print("ISO 调整后的 EV 值: ");
  Serial.println(params.iso);

  Serial.print("快门速度 (T): 1/ ");
  Serial.print(params.shutter);
  Serial.println(" 秒");

  Serial.print("光圈值 (F): ");
  Serial.println(params.aperture);

  Serial.println("------------------------");
}


#endif
