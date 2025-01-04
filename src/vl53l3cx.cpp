#include "vl53l3cx.hpp"

// センサー用インスタンス
VL53LX_Dev_t dev_front, dev_bottom;
VL53LX_DEV Dev_Front = &dev_front;
VL53LX_DEV Dev_Bottom = &dev_bottom;


// XSHUT ピン番号
#define XSHUT_FRONT_PIN 7
#define XSHUT_BOTTOM_PIN 9

uint8_t scan_i2c() {
    printf("I2C scanner. Scanning...\n");
    delay(50);

    uint8_t count = 0;
    for (uint8_t i = 1; i < 127; i++) {
        Wire1.beginTransmission(i);
        if (Wire1.endTransmission() == 0) {
            // アドレスを10進数と16進数で1行に表示する
            // 例: Found address: 60 (0x3C)
            //     ここで %d は 10進、%02X は 16進(2桁0埋め・大文字)
            printf("Found address: %d (0x%02X)\n", i, i);
            count++;
        }
    }

    // 見やすいように改行を入れて合計数を表示
    printf("Found %d device(s).\n", count);

    return count;
}

bool VL53L3CX_Init(VL53LX_DEV Dev, uint8_t xshut_pin, uint8_t new_address) {
    printf("Initializing VL53L3CX...");

    pinMode(xshut_pin, OUTPUT);
    digitalWrite(xshut_pin, LOW); // センサーをリセット
    delay(10);
    digitalWrite(xshut_pin, HIGH); // センサーを有効化
    delay(10);

    // I2C 設定
    Dev->I2cDevAddr = 0x29;
    Dev->comms_type = 1;
    Dev->comms_speed_khz = 400;

    // センサー初期化
    int status = VL53LX_WaitDeviceBooted(Dev);
    if (status != VL53LX_ERROR_NONE) {
        printf("Failed to wait for device booted.");
        return false;
    }

    status = VL53LX_DataInit(Dev);
    if (status != VL53LX_ERROR_NONE) {
        printf("Failed to initialize VL53L3CX sensor.");
        return false;
    }

    // I2C アドレスを変更（必要であれば）
    if (new_address != 0x29) {
        status = VL53LX_SetDeviceAddress(Dev, new_address);
        if (status != VL53LX_ERROR_NONE) {
            printf("Failed to set new I2C address.");
            return false;
        }
        Dev->I2cDevAddr = new_address;
    }

    printf("VL53L3CX initialized successfully.");
    return true;
}
int16_t VL53L3CX_ReadDistance(VL53LX_DEV Dev) {
    VL53LX_MultiRangingData_t rangingData;
    uint8_t dataReady = 0;
    int status = VL53LX_ERROR_NONE;

    // シングルショット測定開始
    status = VL53LX_StartMeasurement(Dev);
    if (status != VL53LX_ERROR_NONE) {
        printf("Failed to start measurement. Status=%d\n", status);
        return -1;
    }

    // 計測完了を待つ(タイムアウト付き)
    uint32_t startMs = millis();
    while (true) {
        status = VL53LX_GetMeasurementDataReady(Dev, &dataReady);
        if (status != VL53LX_ERROR_NONE) {
            printf("Failed to check data ready. Status=%d\n", status);
            return -1;
        }
        if (dataReady == 1) {
            // 計測結果が準備できた
            break;
        }
        // タイムアウト (例: 500ms) を設ける
        if (millis() - startMs > 500) {
            printf("Measurement timeout.\n");
            return -1;
        }
        delay(5); // ポーリング間隔
    }

    // 測定結果を読み出す
    status = VL53LX_GetMultiRangingData(Dev, &rangingData);
    if (status != VL53LX_ERROR_NONE) {
        printf("Failed to get measurement data. Status=%d\n", status);
        return -1;
    }

    // 連続測定でなければ、シングルショット後に停止しておく
    status = VL53LX_StopMeasurement(Dev);
    if (status != VL53LX_ERROR_NONE) {
        printf("Failed to stop measurement. Status=%d\n", status);
        // -1にするかどうかはお好みで
    }

    // 測定結果を返す
    if (rangingData.NumberOfObjectsFound > 0) {
        return rangingData.RangeData[0].RangeMilliMeter;
    } else {
        // デバッグ用に詳しい情報を出力
        printf("No object detected.\n");
        printf("TimeStamp = %lu\n", (unsigned long)rangingData.TimeStamp);
        printf("StreamCount = %u\n", rangingData.StreamCount);
        printf("NumberOfObjectsFound = %u\n", rangingData.NumberOfObjectsFound);
        printf("HasXtalkValueChanged = %u\n", rangingData.HasXtalkValueChanged);
        printf("EffectiveSpadRtnCount = %u\n", rangingData.EffectiveSpadRtnCount);
        return -1;
    }
}
