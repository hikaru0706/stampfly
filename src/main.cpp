#include "ws2812.hpp"
#include <stdio.h>

extern "C" void app_main() {
    printf("Initializing LED control...\n");

    WS2812_HAL led(GPIO_NUM_2, RMT_CHANNEL_0); // GPIO2, RMTチャンネル0
    led.init();
    printf("WS2812 initialized on GPIO2\n");

    const int totalDuration = 60; // 合計ループ時間（秒）
    const int switchInterval = 1; // 色変更の間隔（秒）

    for (int elapsedTime = 0; elapsedTime < totalDuration; elapsedTime += switchInterval) {
        if (elapsedTime % 3 == 0) {
            // 赤色に点灯
            led.setColor(255, 0, 0);
            printf("[%d s] LED set to RED\n", elapsedTime);
        } else if (elapsedTime % 3 == 1) {
            // 緑色に点灯
            led.setColor(0, 255, 0);
            printf("[%d s] LED set to GREEN\n", elapsedTime);
        } else {
            // 青色に点灯
            led.setColor(0, 0, 255);
            printf("[%d s] LED set to BLUE\n", elapsedTime);
        }

        // 1秒待機
        vTaskDelay(pdMS_TO_TICKS(switchInterval * 1000));
    }

    printf("Loop finished after %d seconds.\n", totalDuration);
}
