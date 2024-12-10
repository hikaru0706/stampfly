#include "ws2812.hpp"

extern "C" void app_main() {
    WS2812_HAL led(GPIO_NUM_4, RMT_CHANNEL_0); // 例: GPIO4, RMTチャンネル0
    led.init();

    // 赤色に点灯
    led.setColor(255, 0, 0);

    vTaskDelay(pdMS_TO_TICKS(1000));

    // 緑色に点灯
    led.setColor(0, 255, 0);

    vTaskDelay(pdMS_TO_TICKS(1000));

    // 青色に点灯
    led.setColor(0, 0, 255);
}
