// ws2812_hal.hpp
#pragma once

#include "driver/rmt.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <cstdint>

class WS2812_HAL {
public:
    // コンストラクタで初期化用パラメータを受け取る（GPIOピン、RMTチャネル、RMT分周設定など）
    WS2812_HAL(gpio_num_t gpio_pin, rmt_channel_t rmt_channel, uint8_t clk_div = 2);

    // 初期化処理
    esp_err_t init();

    // カラー設定（RGB）
    esp_err_t setColor(uint8_t red, uint8_t green, uint8_t blue);

private:
    gpio_num_t    _gpio_pin;
    rmt_channel_t _rmt_channel;
    uint8_t       _clk_div;

    // WS2812タイミング定義(ns)
    static constexpr uint32_t WS2812_T0H_NS = 350;
    static constexpr uint32_t WS2812_T0L_NS = 800;
    static constexpr uint32_t WS2812_T1H_NS = 700;
    static constexpr uint32_t WS2812_T1L_NS = 600;
    static constexpr uint32_t WS2812_RESET_US = 50; // reset期間(µs)

    // RMT用の1tickあたりのnsを計算するのに使用する
    // RMTは80MHzが基本クロック、clk_divで分周するのでtickは (1/80MHz)*div [s] = (12.5ns * div)
    // 今回はdiv=2なので1tickあたり25nsになる
    // durationはtick数で指定するため、nsを25nsで割ることでtick数を求める
    static constexpr uint32_t RMT_TICK_NS = 25;

    // RGBデータをRMT用のitem配列に変換するヘルパ関数
    void fillItemsForColor(rmt_item32_t* items, uint8_t green, uint8_t red, uint8_t blue);

    // リセットシグナル送信（実際にはWS2812規定のため、LED側でデータラッチのためのディレイを入れるだけ）
    void sendReset();
};

