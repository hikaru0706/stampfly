#include <Arduino.h>  // Arduinoの基本機能
#include <Wire.h>     // I2C通信
#include "vl53lx_api.h"  // VL53L3CX用API
#include "vl53lx_platform.h"  // プラットフォーム固有の設定
#include "vl53l3cx.hpp"  // センサーの初期化/操作用ヘッダー
#include <stdio.h>

// I2Cおよびセンサーピンの定義
#define SDA_PIN 3
#define SCL_PIN 4

#define INTERRUPT_FRONT_PIN 6
#define XSHUT_FRONT_PIN 7
#define INTERRUPT_BOTTOM_PIN 8
#define XSHUT_BOTTOM_PIN 9

void setup() {
    Serial.begin(115200);
    delay(2000); //安定化
    printf("Setup started...\n");

    // I2C 初期化
    Wire1.begin(SDA_PIN, SCL_PIN,400000UL);  // SDAとSCLピンを指定して初期化
    printf("I2C initialized.\n");
    delay(500); // バス安定化のための短い遅延
    // I2C デバイススキャン
    if (scan_i2c() == 0) {
        printf("No I2C devices found. Halting.");
        while (true) { delay(100); }
    }
    digitalWrite(XSHUT_BOTTOM_PIN,LOW);
    delay(10);
    // フロントセンサーの初期化
    if (!VL53L3CX_Init(Dev_Front, XSHUT_FRONT_PIN,INTERRUPT_FRONT_PIN, 0x2A)) {
        printf("Failed to initialize Front sensor. Halting.\n");
        while (true) { delay(100); }
    }

    digitalWrite(XSHUT_BOTTOM_PIN,HIGH);
    delay(10);
    // // ボトムセンサーの初期化
    if (!VL53L3CX_Init(Dev_Bottom, XSHUT_BOTTOM_PIN,INTERRUPT_BOTTOM_PIN,0x29)) {
        printf("Failed to initialize Bottom sensor. Halting.\n");
        while (true) { delay(100); }
    }

    printf("Both sensors initialized successfully.\n");
}

void loop() {
    int16_t distance_front = VL53L3CX_ReadDistance(Dev_Front);
    int16_t distance_bottom = VL53L3CX_ReadDistance(Dev_Bottom);

    if (distance_front >= 0) {
        printf("Front Distance: %d mm\n", distance_front); // 数値を表示
    } else {
        printf("Failed to read Front distance. :%d \n",distance_front);
    }

    if (distance_bottom >= 0) {
        printf("Bottom Distance: %d mm\n",distance_bottom);
    } else {
        printf("Failed to read Bottom distance. :%d \n",distance_bottom);
    }

    delay(500);
}
