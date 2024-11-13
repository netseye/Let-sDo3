#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>
#include "CalculateParams.h"

extern bool deviceConnected;

// Use dedicated hardware SPI pins
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

GFXcanvas16 canvas(240, 135);

void tftInit() {
  Serial.print(F("Hello! Feather TFT Test"));

  // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // initialize TFT
  tft.init(135, 240);  // Init ST7789 240x135
  tft.setRotation(3);
  tft.fillScreen(ST77XX_GREEN);

  Serial.println(F("Initialized"));
}

void flashScreen() {
   tft.fillScreen(ST77XX_WHITE);
    delay(500);
}

// void displayParameters(int aperture, int shutter_speed, int lux, int iso, ExposureMode exposureMode) {

//   static float lastAperture = -1;
//   static int lastShutterSpeed = -1;
//   static unsigned int lastLux = 0;
//   static float lastIso = -1;
//   static bool lastDeviceConnected = false;

//   if (lastAperture != aperture || lastShutterSpeed != shutter_speed || lastLux != lux || lastIso != iso || lastDeviceConnected != deviceConnected) {

//     lastAperture = aperture;
//     lastShutterSpeed = shutter_speed;
//     lastLux = lux;
//     lastIso = iso;
//     lastDeviceConnected = deviceConnected;

//     tft.fillScreen(deviceConnected ? ST77XX_BLUE : ST77XX_BLACK);
//     tft.setTextColor(ST77XX_WHITE);
//     tft.setTextSize(2);

//     // 显示光圈值
//     tft.setCursor(10, 20);
//     tft.printf("Aperture: f/%.1f", aperture / 10.0f);

//     // 显示快门速度
//     tft.setCursor(10, 50);
//     tft.printf("Shutter: 1/%d", shutter_speed);

//     // 显示 Lux 值
//     tft.setCursor(10, 80);
//     tft.printf("Lux: %u", lux);

//     // 显示 ISO 值
//     tft.setCursor(10, 110);
//     tft.printf("ISO: %.2f", iso);
//   }

//     //  display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
// }


void displayParameters(int aperture, int shutter_speed, int lux, int iso, ExposureMode exposureMode) {

  static int lastAperture = -1;
  static int lastShutterSpeed = -1;
  static unsigned int lastLux = 0;
  static float lastIso = -1;
  static bool lastDeviceConnected = false;
  static ExposureMode lastExposureMode= AUTO;

  if (lastAperture != aperture || lastShutterSpeed != shutter_speed || lastLux != lux || lastIso != iso || lastDeviceConnected != deviceConnected ||lastExposureMode != exposureMode) {

    lastAperture = aperture;
    lastShutterSpeed = shutter_speed;
    lastLux = lux;
    lastIso = iso;
    lastDeviceConnected = deviceConnected;
    lastExposureMode = exposureMode;

    // 设置背景颜色
    tft.fillScreen(deviceConnected ? ST77XX_BLUE : ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);

    // 显示光圈值
    tft.setCursor(10, 5);
    tft.printf("Aperture: f/%.1f", aperture / 10.0f);

    // 显示快门速度
    tft.setCursor(10, 35);
    tft.printf("Shutter: 1/%d", shutter_speed);

    // 显示 Lux 值
    tft.setCursor(10, 65);
    tft.printf("Lux: %u", lux);

    // 显示 ISO 值
    tft.setCursor(10, 95);
    tft.printf("ISO: %d", iso);

    // 在右上角显示曝光模式
    const char* modeStr = "";
    uint16_t bgColor = ST77XX_BLACK;  // 默认背景色

    switch (exposureMode) {
      case AUTO:
        modeStr = "AE";  // 自动曝光
        bgColor = ST77XX_RED;  // 红色背景
        break;
      case APERTURE:
        modeStr = "AV";  // 光圈优先
        bgColor = ST77XX_GREEN;  // 绿色背景
        break;
      case SHUTTER:
        modeStr = "TV";  // 快门优先
        bgColor = ST77XX_MAGENTA;  // 紫色背景
        break;
    }

    // 设置右上角显示区域的背景色
    tft.fillRect(240 - 40, 0, 40, 20, bgColor);
    tft.setCursor(240 - 35, 5);  // 设置文本位置
    tft.setTextColor(ST77XX_WHITE);  // 文字为白色
    tft.setTextSize(2);
    tft.print(modeStr);
    delay(16);
  }
}
