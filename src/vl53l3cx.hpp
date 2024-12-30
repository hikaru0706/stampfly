#ifndef VL53L3CX_HPP
#define VL53L3CX_HPP

#include <Arduino.h>
#include <Wire.h>
#include "vl53lx_api.h"
#include "vl53lx_platform.h"

// 定義: VL53L3CX ピン番号
#define SDA_PIN 3
#define SCL_PIN 4
#define XSHUT_PIN 7

// プロトタイプ宣言
uint8_t scan_i2c();
bool VL53L3CX_Init(VL53LX_DEV Dev);
int16_t VL53L3CX_ReadDistance(VL53LX_DEV Dev);

#endif // VL53L3CX_HPP
