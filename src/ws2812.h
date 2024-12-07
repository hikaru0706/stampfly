#ifndef WS2812_H
#define WS2812_H

#include "driver/rmt.h"

// RMTのチャンネルとGPIO設定
#define WS2812_RMT_CHANNEL RMT_CHANNEL_0

// WS2812制御用関数
void ws2812_init(gpio_num_t gpio_pin);
void ws2812_set_color(uint8_t red, uint8_t green, uint8_t blue);

#endif // WS2812_H
