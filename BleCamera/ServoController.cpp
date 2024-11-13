#include "ServoController.h"
#include <M5_UNIT_8SERVO.h>

M5_UNIT_8SERVO unit_8servo;

void servoInit() {
  unit_8servo.begin();
  unit_8servo.setAllPinMode(SERVO_CTL_MODE);
}

void rotateServos(int angle) {
  for (uint8_t i = 0; i < 8; i++) {
    unit_8servo.setServoAngle(i, angle);
  }
}


void shutter() {
  unit_8servo.setServoAngle(1, 180);
  unit_8servo.setServoAngle(2, 180);
  vTaskDelay(500);                  // 等待 500ms 防止重复触发
  unit_8servo.setServoAngle(1, 0);  // 设定所有舵机回正到 0 度
  unit_8servo.setServoAngle(2, 0);  // 设定所有舵机回正到 0 度
}