#include <Arduino.h>
#include "LedController.h"
#include "CalculateParams.h"
#include "DisplayManager.h"
#include "ServoController.h"

extern ExposureMode exposureMode; // 曝光模式

#define D0_BUTTON_PIN 0
#define D1_BUTTON_PIN 1
#define D2_BUTTON_PIN 2
#define GREEN_BUTTON_PIN 36
#define RED_BUTTON_PIN 26

void buttonInit() {
    pinMode(D0_BUTTON_PIN, INPUT_PULLUP);
    pinMode(D1_BUTTON_PIN, INPUT_PULLDOWN);
    pinMode(D2_BUTTON_PIN, INPUT_PULLDOWN);
    pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
}


void buttonTick() {
    // D0 按键按下 (INPUT_PULLUP，按下时为 LOW)
    if (digitalRead(D0_BUTTON_PIN) == LOW) {
        Serial.println("D0 button pressed");

        switch (exposureMode) {
          case AUTO:
            exposureMode = APERTURE;
            break;
          case APERTURE:
            exposureMode = SHUTTER;
            break;
          case SHUTTER:
            exposureMode = AUTO;
            break;
        }
        delay(200);
    }

    // D1 按键按下 (INPUT_PULLDOWN，按下时为 HIGH)
    if (digitalRead(D1_BUTTON_PIN) == HIGH) {
        Serial.println("D1 button pressed");   
              ledOn();
    }

    // D2 按键按下 (INPUT_PULLDOWN，按下时为 HIGH)
    if (digitalRead(D2_BUTTON_PIN) == HIGH) {
        Serial.println("D2 button pressed");
        flashScreen();              
        shutter();    
    }
}
