#include <Arduino.h>
#include "CalculateParams.h"
#include "BLEManager.h"
#include "ServoController.h"
#include "DisplayManager.h"
#include "ButtonController.h"
#include "ledController.h"

extern bool deviceConnected;
extern int currentAperture;       // 光圈值，如 f/1.8
extern int currentShutterSpeed ; // 快门速度，如 1/1000 秒
extern int currentISO;           // ISO 值
extern ExposureMode exposureMode; // 曝光模式

void setup() {
    Serial.begin(115200);
    buttonInit();
    calculateInit();
    bleInit();
    tftInit();
    servoInit();
    ledInit();
    Serial.println("Start");
}

void loop() {
    buttonTick();
    calculate();
    displayParameters(currentAperture, currentShutterSpeed, readLUX(), currentISO, exposureMode);
    delay(10);
}
