#include <NimBLEDevice.h>
#include <M5_UNIT_8SERVO.h>
#include "DisplayManager.h"
#include "ServoController.h"

NimBLEServer *pServer;
NimBLEService *pService;
NimBLECharacteristic *pPhotoCharacteristic;
NimBLECharacteristic *pApertureCharacteristic;
NimBLECharacteristic *pShutterCharacteristic;
NimBLECharacteristic *pIsoCharacteristic;

// BLE 客户端连接状态
bool deviceConnected = false;
int currentAperture = 18;       // 模拟光圈值，如 f/1.8
int currentShutterSpeed = 1000; // 模拟快门速度，如 1/1000 秒
int currentISO = 100;           // 模拟 ISO 值

// 服务和特性的 UUID
const char *SERVICE_UUID = "0000fea6-0000-1000-8000-00805f9b34fb";
const char *PHOTO_CHAR_UUID = "b5f90072-aa8d-11e3-9046-0002a5d5c51b";
const char *APERTURE_CHAR_UUID = "0000fea7-0000-1000-8000-00805f9b34fb";
const char *SHUTTER_CHAR_UUID = "0000fea8-0000-1000-8000-00805f9b34fb";
const char *ISO_CHAR_UUID = "0000fea9-0000-1000-8000-00805f9b34fb";

// 通知当前设置
void notifyCurrentSettings() {
  if (deviceConnected) {
    // 光圈通知
    char apertureValue[10];
    snprintf(apertureValue, sizeof(apertureValue), "f/%.1f", currentAperture / 10.0);
    pApertureCharacteristic->setValue(apertureValue);
    pApertureCharacteristic->notify();

    // 快门速度通知
    char shutterValue[10];
    snprintf(shutterValue, sizeof(shutterValue), "1/%d", currentShutterSpeed);
    pShutterCharacteristic->setValue(shutterValue);
    pShutterCharacteristic->notify();

    // ISO 通知
    char isoValue[10];
    snprintf(isoValue, sizeof(isoValue), "ISO %d", currentISO);
    pIsoCharacteristic->setValue(isoValue);
    pIsoCharacteristic->notify();

    Serial.printf("通知更新: 光圈: %s, 快门速度: %s, ISO: %s\n", apertureValue, shutterValue, isoValue);
  }
}

// BLE 服务回调
class ServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer) {
    Serial.println("BLE 客户端已连接");
    deviceConnected = true;
    notifyCurrentSettings(); // 客户端连接后立即通知当前数据
  }

  void onDisconnect(NimBLEServer* pServer) {
    Serial.println("BLE 客户端已断开连接");
    deviceConnected = false;
    pServer->startAdvertising(); // 重新开始广播
  }
};

// 拍照特性写入回调
class PhotoCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 0 && rxValue[0] == 0x03 && rxValue[1] == 0x01) {
      Serial.println("执行拍照指令");
      Serial.printf("当前光圈: f/%.1f, 当前快门速度: 1/%d 秒, ISO: %d\n",
                    currentAperture / 10.0, currentShutterSpeed, currentISO);

      flashScreen();              
      shutter();             
    }
  }
};

// 光圈控制特性写入回调
class ApertureCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 0) {
      currentAperture = (int)rxValue[0];
      Serial.printf("设置光圈为: f/%.1f\n", currentAperture / 10.0);
      notifyCurrentSettings(); // 更新通知
    }
  }
};

// 快门控制特性写入回调
class ShutterCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 1) {
      currentShutterSpeed = (int)rxValue[0] * 100 + (int)rxValue[1] * 10;
      Serial.printf("设置快门速度为: 1/%d 秒\n", currentShutterSpeed);
      notifyCurrentSettings(); // 更新通知
    }
  }
};

// ISO 控制特性写入回调
class IsoCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 0) {
      currentISO = (int)rxValue[0] * 100;
      Serial.printf("设置 ISO 为: %d\n", currentISO);
      notifyCurrentSettings(); // 更新通知
    }
  }
};


// 初始化 BLE
void bleInit() {
  NimBLEDevice::init("DigiKey&EEPW");

  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  // 创建 GoPro 服务
  pService = pServer->createService(SERVICE_UUID);

  // 创建拍照特性
  pPhotoCharacteristic = pService->createCharacteristic(
    PHOTO_CHAR_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE
  );
  pPhotoCharacteristic->setCallbacks(new PhotoCharacteristicCallbacks());

  // 创建光圈特性
  pApertureCharacteristic = pService->createCharacteristic(
    APERTURE_CHAR_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
  );
  pApertureCharacteristic->setCallbacks(new ApertureCharacteristicCallbacks());

  // 创建快门速度特性
  pShutterCharacteristic = pService->createCharacteristic(
    SHUTTER_CHAR_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
  );
  pShutterCharacteristic->setCallbacks(new ShutterCharacteristicCallbacks());

  // 创建 ISO 特性
  pIsoCharacteristic = pService->createCharacteristic(
    ISO_CHAR_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
  );
  pIsoCharacteristic->setCallbacks(new IsoCharacteristicCallbacks());

  // 启动服务并开始广播
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("BLE 广播开始，等待客户端连接...");
}