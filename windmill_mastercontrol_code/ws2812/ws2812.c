//
// Created by 86136 on 2023/3/5.
//

#include "ws2812.h"


unsigned int str[13][24];
unsigned int color_red = 50 << 8;       /*ºìÉ«*/
unsigned int color_blue = 50 << 16;     /*À¶É«*/


inline void SetSingleColor(unsigned char led[24], unsigned int color) {
    for (unsigned char i = 0; i < 24; i++) {
        if ((color >> i) & 1) led[i] = WS2812_H;
        else led[i] = WS2812_L;
    }
}

void SetColor(unsigned int color, unsigned char leds[][24], unsigned int length) {
    for (unsigned int i = 0; i < length; i++)
        SetSingleColor(leds[i], color);
}

