#include <Arduino.h>
#include "vl53l3cx.hpp"

VL53LX_Dev_t dev;
VL53LX_DEV   Dev = &dev;

void setup() {
    Serial.begin(115200);

    Serial.println("Starting setup ...");

    // I2C 初期化
        // I2C 初期化
    if (!Wire1.begin(SDA_PIN, SCL_PIN,400000UL)) {
        Serial.println("I2C initialization failed! Halting.");
        while (true) { delay(100); }
    }
    Serial.println("I2C initialized.");



    // I2C デバイススキャン
    if (scan_i2c() == 0) {
        Serial.println("No I2C devices found. Halting.");
        while (true) { delay(100); }
    }

    // VL53L3CX センサー初期化
    if (!VL53L3CX_Init(Dev)) {
        Serial.println("Failed to initialize VL53L3CX. Halting.");
        while (true) { delay(100); }
    }
}

void loop() {
    static unsigned long lastMillis = 0;
    unsigned long currentMillis = millis();

    // デバッグ用: ループのタイミング確認
    Serial.print("Loop interval: ");
    Serial.println(currentMillis - lastMillis);
    lastMillis = currentMillis;

    // 測距データ取得
    int16_t distance = VL53L3CX_ReadDistance(Dev);
    if (distance >= 0) {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" mm");
    } else {
        Serial.println("Failed to read distance.");
    }

    delay(500);
}
