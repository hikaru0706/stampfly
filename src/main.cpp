#include <Arduino.h>
#include <Wire.h>
#include "vl53lx_class.h"

#define SDA_PIN 13
#define SCL_PIN 15
#define XSHUT_PIN 14

VL53LX sensor(&Wire, XSHUT_PIN);

void setup() {
  Serial.begin(115200);

  // I2Cを初期化
  Wire.begin(SDA_PIN, SCL_PIN);

  // シャットダウンピン設定
  pinMode(XSHUT_PIN, OUTPUT);
  digitalWrite(XSHUT_PIN, HIGH);

  if (sensor.VL53LX_SetDeviceAddress(0x52) != 0) {
    Serial.println("Failed to set I2C address");
  } else {
    Serial.println("I2C address set to 0x52");
  }

  // センサーをオン (ライブラリによっては begin() が存在するかも)
  sensor.VL53LX_On();
  // sensor.begin(); // もし begin() があれば
  delay(50);
}

void loop() {
  VL53LX_MultiRangingData_t rangingData;

  // 計測開始
  if (sensor.VL53LX_StartMeasurement() != 0) {
    Serial.println("Failed to start measurement");
    return;
  }

  // 計測結果取得
  if (sensor.VL53LX_GetMultiRangingData(&rangingData) != 0) {
    Serial.println("Failed to get measurement data");
    return;
  }

  if (rangingData.NumberOfObjectsFound > 0) {
    int distance = rangingData.RangeData[0].RangeMilliMeter;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  } else {
    Serial.println("No object detected");
  }

  delay(500);
}
