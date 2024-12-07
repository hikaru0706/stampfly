#include "ws2812.h"
#include <string.h>

#define WS2812_T0H_NS 350  // T0H時間（350ns）
#define WS2812_T0L_NS 800  // T0L時間（800ns）
#define WS2812_T1H_NS 700  // T1H時間（700ns）
#define WS2812_T1L_NS 600  // T1L時間（600ns）

#define WS2812_RESET_US 50 // リセット信号の時間（50µs）

void ws2812_init(gpio_num_t gpio_pin) {
    // RMT設定
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(gpio_pin, WS2812_RMT_CHANNEL);
    config.clk_div = 2; // クロック分周設定
    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(WS2812_RMT_CHANNEL, 0, 0));
}

void ws2812_set_color(uint8_t red, uint8_t green, uint8_t blue) {
    rmt_item32_t items[24]; // WS2812のデータ用配列（1LED=24ビット）

    // データ順序（Green, Red, Blue）
    uint8_t colors[3] = {green, red, blue};
    int index = 0;

    for (int i = 0; i < 3; i++) { // 各色（Green, Red, Blue）
        for (int bit = 7; bit >= 0; bit--) { // 各ビット
            if (colors[i] & (1 << bit)) {
                // '1'信号
                items[index].level0 = 1;
                items[index].duration0 = WS2812_T1H_NS / 25;
                items[index].level1 = 0;
                items[index].duration1 = WS2812_T1L_NS / 25;
            } else {
                // '0'信号
                items[index].level0 = 1;
                items[index].duration0 = WS2812_T0H_NS / 25;
                items[index].level1 = 0;
                items[index].duration1 = WS2812_T0L_NS / 25;
            }
            index++;
        }
    }

    // RMTにデータ送信
    ESP_ERROR_CHECK(rmt_write_items(WS2812_RMT_CHANNEL, items, 24, true));
    ESP_ERROR_CHECK(rmt_wait_tx_done(WS2812_RMT_CHANNEL, portMAX_DELAY));

    // リセット信号を送る
    vTaskDelay(pdMS_TO_TICKS(WS2812_RESET_US));
}
