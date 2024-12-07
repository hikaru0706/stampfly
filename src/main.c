#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt.h"
#include "ws2812.h"

#define LED_PIN GPIO_NUM_18 // WS2812のデータピン

void app_main() {
    // WS2812の初期化
    ws2812_init(LED_PIN);

    while (1) {
        // 赤色に点灯
        ws2812_set_color(255, 0, 0); // 赤
        vTaskDelay(pdMS_TO_TICKS(500)); // 500ms待機

        // 緑色に点灯
        ws2812_set_color(0, 255, 0); // 緑
        vTaskDelay(pdMS_TO_TICKS(500)); // 500ms待機

        // 青色に点灯
        ws2812_set_color(0, 0, 255); // 青
        vTaskDelay(pdMS_TO_TICKS(500)); // 500ms待機
    }
}
