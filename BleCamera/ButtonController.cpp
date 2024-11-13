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
    // RED 按键按下 (INPUT_PULLUP，按下时为 LOW)
    // if (digitalRead(RED_BUTTON_PIN) == LOW) {
    //     Serial.println("RED button pressed");
    //     // rotateServos(180);
    //     // delay(500);
    //     // rotateServos(0);
    // }

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
        // ledOn();
    }

    // D1 按键按下 (INPUT_PULLDOWN，按下时为 HIGH)
    if (digitalRead(D1_BUTTON_PIN) == HIGH) {
        Serial.println("D1 button pressed");   
    }

    // D2 按键按下 (INPUT_PULLDOWN，按下时为 HIGH)
    if (digitalRead(D2_BUTTON_PIN) == HIGH) {
        Serial.println("D2 button pressed");
        flashScreen();              
        shutter();    
    }

    // GREEN 按键按下 (INPUT_PULLUP，按下时为 LOW)
    // if (digitalRead(GREEN_BUTTON_PIN) == LOW) {
    //     Serial.println("GREEN button pressed");
    //     // digitalWrite(LED_PIN, HIGH); // 点亮 LED
    // } else {
    //     // digitalWrite(LED_PIN, LOW);  // 熄灭 LED
    // }
}



// void buttonTick(){
//     //  // Serial.println("loop");
//     // if (digitalRead(RED_BUTTON_PIN) == LOW) {
//     //     rotateServos(180);
//     //     delay(500);
//     //     rotateServos(0);
//     // }

//       // RED 按键按下
//     if (digitalRead(RED_BUTTON_PIN) == LOW) {
//         // rotateServos(180);
//         // delay(500);
//         // rotateServos(0);
//     }

//     // D0 按键按下，舵机转到 0 度
//     if (digitalRead(D0_BUTTON_PIN) == LOW) {
//         // rotateServos(0);
//         ledOn();
//         Serial.print("D0");
//     }

//     // D1 按键按下，舵机转到 90 度
//     if (digitalRead(D1_BUTTON_PIN) == LOW) {
//         // rotateServos(90);
//         Serial.print("D1 ");
//     }

//     // D2 按键按下，舵机转到 180 度
//     if (digitalRead(D2_BUTTON_PIN) == LOW) {
//         // rotateServos(180);
//           Serial.print("D2 ");
//     }

//     // GREEN 按键按下，点亮 LED
//     if (digitalRead(GREEN_BUTTON_PIN) == LOW) {
//         // digitalWrite(LED_PIN, HIGH); // 点亮 LED
//     } else {
//         // digitalWrite(LED_PIN, LOW);  // 熄灭 LED
//     }
// }