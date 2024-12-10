// ws2812_hal.cpp
#include "ws2812.hpp"
#include "esp_check.h"

WS2812_HAL::WS2812_HAL(gpio_num_t gpio_pin, rmt_channel_t rmt_channel, uint8_t clk_div)
: _gpio_pin(gpio_pin),
  _rmt_channel(rmt_channel),
  _clk_div(clk_div)
{
}

esp_err_t WS2812_HAL::init() {
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(_gpio_pin, _rmt_channel);
    config.clk_div = _clk_div;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(_rmt_channel, 0, 0));

    // 初期化後に一度リセット信号を送る（念のため）
    sendReset();

    return ESP_OK;
}

esp_err_t WS2812_HAL::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    // 24bit（Green, Red, Blue）
    rmt_item32_t items[24];
    fillItemsForColor(items, green, red, blue);

    // データ送信
    ESP_ERROR_CHECK(rmt_write_items(_rmt_channel, items, 24, true));
    ESP_ERROR_CHECK(rmt_wait_tx_done(_rmt_channel, portMAX_DELAY));

    // リセット送信(LEDにデータを反映)
    sendReset();

    return ESP_OK;
}

void WS2812_HAL::fillItemsForColor(rmt_item32_t* items, uint8_t green, uint8_t red, uint8_t blue) {
    uint8_t colors[3] = {green, red, blue};
    int index = 0;

    for (int i = 0; i < 3; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            bool isOne = (colors[i] & (1 << bit)) != 0;
            if (isOne) {
                // '1'のとき
                items[index].level0    = 1;
                items[index].duration0 = WS2812_T1H_NS / RMT_TICK_NS; // T1H
                items[index].level1    = 0;
                items[index].duration1 = WS2812_T1L_NS / RMT_TICK_NS; // T1L
            } else {
                // '0'のとき
                items[index].level0    = 1;
                items[index].duration0 = WS2812_T0H_NS / RMT_TICK_NS; // T0H
                items[index].level1    = 0;
                items[index].duration1 = WS2812_T0L_NS / RMT_TICK_NS; // T0L
            }
            index++;
        }
    }
}

void WS2812_HAL::sendReset() {
    // WS2812のリセットは、50µs以上LOWを維持すればよいとされている
    // RMTで特に信号を送らず、ここで単純にdelayで対応する
    vTaskDelay(pdMS_TO_TICKS(WS2812_RESET_US));
}
