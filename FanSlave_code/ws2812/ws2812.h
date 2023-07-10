//
// Created by 86136 on 2023/3/5.
//

#ifndef WINDMILL_MASTERCONTROL_CODE_WS2812_H
#define WINDMILL_MASTERCONTROL_CODE_WS2812_H


#define DMA_CHANNEL_FLOW     HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_3, (uint32_t *) led_flow, (BOARD_LENGTH + 1) * 24)
#define DMA_CHANNEL_WS2812       HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *) ws2812, (led_DOWN_NUM+led_UP_NUM + 1) * 24)
#define DMA_CHANNEL_CIRCLE      HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_2, (uint32_t *) ws2812, (WS2812_LEN + 1) * 24)


#define WS2812_H 52
#define WS2812_L 21

#define BOARD_LENGTH (32*8)
#define WS2812_LEN (440)
#define led_UP_NUM 140
#define led_DOWN_NUM 102

#define WS2812_OFF 0
#define WS2812_RED 50<<8
#define WS2812_BLUE 51<<16

#define WS2812_RED_LOW 0B01000000<<8
#define WS2812_BLUE_LOW 0B01000000<<16

#define WINDMILL_INIT_COLOR WS2812_RED


enum ws2812_circle_state_e {
    CIRCLE_OFF,
    CIRCLE_ON,
    CIRCLE_HIT
};

enum ws2812_state_e {
    UP,
    DOWN,
    ALL,
};

enum {
    OFF = 1,
    READY_HIT = 2,
    HIT = 3,
    BLINK = 4
};

void SetColor(unsigned char leds[][24], unsigned int color, unsigned int length);

void SetSingleColor(unsigned char led[24], unsigned int color);

void SetFlowColor(unsigned long color, unsigned char leds[BOARD_LENGTH][24]);

void AroundColor_Set(unsigned char leds[][24], unsigned int color, unsigned int length, enum ws2812_state_e state);

void WS2812_Init();

void WS2812_Load();

#endif //WINDMILL_MASTERCONTROL_CODE_WS2812_H
