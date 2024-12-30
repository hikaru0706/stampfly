#include "vl53l3cx.hpp"

uint8_t scan_i2c() {
    Serial.println("I2C scanner. Scanning...");
    delay(50);
    byte count = 0;
    for (uint8_t i = 1; i < 127; i++) {
        Wire.beginTransmission(i);        // I2C アドレスに通信開始
        if (Wire.endTransmission() == 0) { // 応答 (ACK) を確認
            Serial.print("Found address: ");
            Serial.print(i, DEC);
            Serial.print(" (0x");
            Serial.print(i, HEX);
            Serial.println(")");
            count++;
        }
    }
    Serial.print("Found ");
    Serial.print(count, DEC);  // 見つかったデバイスの数
    Serial.println(" device(s).");
    return count;
}

bool VL53L3CX_Init(VL53LX_DEV Dev) {
    Serial.println("Initializing VL53L3CX...");

    // XSHUT ピンの初期化
    pinMode(XSHUT_PIN, OUTPUT);
    digitalWrite(XSHUT_PIN, HIGH);
    delay(10);

    // I2C アドレスと通信設定
    Dev->I2cDevAddr = 0x29;    // デフォルトの I2C アドレス
    Dev->comms_type = 1;       // I2C 通信
    Dev->comms_speed_khz = 400; // 400kHz

    // センサー起動待ち & 初期化
    int status = VL53LX_WaitDeviceBooted(Dev);
    if (status != VL53LX_ERROR_NONE) {
        Serial.println("Failed to wait for device booted.");
        return false;
    }

    status = VL53LX_DataInit(Dev);
    if (status != VL53LX_ERROR_NONE) {
        Serial.println("Failed to initialize VL53L3CX sensor.");
        return false;
    }

    Serial.println("VL53L3CX initialized successfully.");
    return true;
}

int16_t VL53L3CX_ReadDistance(VL53LX_DEV Dev) {
    VL53LX_MultiRangingData_t rangingData;

    // 測距開始
    int status = VL53LX_StartMeasurement(Dev);
    if (status != VL53LX_ERROR_NONE) {
        Serial.println("Failed to start measurement.");
        return -1;
    }

    // 測距データ取得
    status = VL53LX_GetMultiRangingData(Dev, &rangingData);
    if (status != VL53LX_ERROR_NONE) {
        Serial.println("Failed to get measurement data.");
        return -1;
    }

    // 測距結果を返す
    if (rangingData.NumberOfObjectsFound > 0) {
        return rangingData.RangeData[0].RangeMilliMeter;
    } else {
        Serial.println("No object detected.");
        return -1;
    }
}
