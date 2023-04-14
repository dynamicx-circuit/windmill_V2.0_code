/**
  ******************************************************************************
  * @file    ws2812.c
  * @author  kade
  * @brief   This file provides some function to control the ws2812 led light strip
  ******************************************************************************
  *
 [..]
   (#) 在该文件中使用了两路DMA来控制WS2812，一路用来控制8*32的灯板，一路用来控制四周的灯带。

   (#) 在调用之前，需要先初始化灯带的状态，在此将灯带全部关闭。

   (#) 初始化完成之后，进入准备状态，等待主板发送的信号，以此来确定。

   (#) 设置led灯的状态：
       (++) 设置8*32灯板的状态，当接等待击打的状态时，8*32灯板会显示流动的箭头。当扇叶被击中时，整个灯板亮起。
       (++) 设置四周灯带的状态，当处于等待击打的状态时，上部分的灯带会亮，而下部分的灯带会灭掉。
            被打中之后，四周的灯带都会亮起。
   */

#include"ws2812.h"
#include<string.h>
#include"main.h"
#include"tim.h"
#include "spi.h"
#include "oled.h"
#include "HX711.h"
#include "clock.h"

unsigned int color = WINDMILL_INIT_COLOR;


unsigned char led_flow[BOARD_LENGTH + 1][24];
unsigned char ws2812[WS2812_LEN + 1][24];
///*风车箭头的数组*/
unsigned char flow[48] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 1, 0,
        0, 1, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 0
};

/**
 * 设置单个led的颜色
 * @param led   led灯所在的数组，使用24位来表示，一个灯使用的数组要有24位
 * @param color 要设置的颜色
 */
inline void SetSingleColor(unsigned char led[24], unsigned int color) {
    for (unsigned char i = 0; i < 24; i++) {
        if ((color >> i) & 1) led[i] = WS2812_H;
        else led[i] = WS2812_L;
    }
}

/**
 * 设置一串灯的颜色
 * @param color
 * @param leds led灯所在的数组，使用24位来表示，一个灯使用的数组要有24位
 * @param length 灯带的长度
 */
void SetColor(unsigned char leds[][24], unsigned int color, unsigned int length) {
    for (unsigned int i = 0; i < length; i++)
        SetSingleColor(leds[i], color);
}

/**
 * 设置箭头
 * @param color 颜色
 * @param leds 箭头对应的数组
 */
void SetFlowColor(unsigned long color, unsigned char leds[BOARD_LENGTH][24]) {
    unsigned char j = 0;
    for (unsigned long i = 0; i < BOARD_LENGTH / 8; i++) {
        for (unsigned char n = 0; n < 8; n++) {
            if (flow[8 * j + n]) SetSingleColor(leds[8 * i + n], color);
            else SetSingleColor(leds[8 * i + n], WS2812_OFF);
        }
        j++;
        if (j == 6) j = 0;
    }
}

void SetFlowBoardColor(unsigned long color, unsigned char leds[BOARD_LENGTH][24]) {
    for (unsigned long i = 0; i < BOARD_LENGTH; i++) {
        SetSingleColor(leds[i], color);
    }
}

/**
 * 更新箭头，使其流动
 */
void LoadFlowColor(unsigned long color) {
    uint8_t flow_buffer[40];
    memcpy(flow_buffer, flow, 40);
    memcpy(flow, flow + 40, 8);
    memcpy(flow + 8, flow_buffer, 40);
    SetFlowColor(color, led_flow);
}

/**
 * 初始化四周灯珠的颜色
 * @param leds
 * @param color
 * @param length
 */
void AroundColor_Init(unsigned char leds[][24], unsigned int color, unsigned int length) {
    for (int i = 0; i < length; i++) {
        SetSingleColor(leds[i], color);
    }
}


void AroundColor_Set(unsigned char leds[][24], unsigned int color, unsigned int length, enum ws2812_state state) {
    uint32_t color_temp;
    if (color == WS2812_RED) {
        color_temp = color + (5 << 8);
    } else if (color == WS2812_BLUE) {
        color_temp = color + (5 << 16);
    } else if (color == WS2812_OFF) {
        color_temp = 0;
    }

    if (state == ALL) {
        for (int i = 0; i < length; i++) {

            if (i < 84) {
                SetSingleColor(leds[i], color_temp);
            } else if (i > 94 && i < 138) {
                SetSingleColor(leds[i], color_temp);
            } else if (i > 191 && i < 212) {
                SetSingleColor(leds[i], color_temp);
            } else if (i > 218 && i < 240) {
                SetSingleColor(leds[i], color_temp);
            } else {
                SetSingleColor(leds[i], color);
            }

        }

    } else if (state == DOWN) {
        for (int i = 0; i < length; i++) {
            if (i >= 13 && i <= 24) {
                SetSingleColor(leds[i], color_temp);
            } else if (i >= 84 && i <= 94) {
                SetSingleColor(leds[i], color);
            } else if (i > 191 && i < 212) {
                SetSingleColor(leds[i], color_temp);
            } else if (i > 218 && i < 240) {
                SetSingleColor(leds[i], color_temp);
            } else if (i < led_UP_NUM)
                SetSingleColor(leds[i], WS2812_OFF);
            else {
                SetSingleColor(leds[i], color
                );
            }
        }
    } else if (state == UP) {
        for (
                int i = 0;
                i < length;
                i++) {
            if (i < led_UP_NUM)
                SetSingleColor(leds[i], color
                );
            else {
                SetSingleColor(leds[i],
                               WS2812_OFF);
            }
        }
    }
}

void Blink_led(unsigned int color) {

    /*亮*/
    SetFlowBoardColor(color, led_flow);
    DMA_CHANNEL_FLOW;
    HAL_Delay(10);
    AroundColor_Set(ws2812, color, WS2812_LEN, ALL);
    DMA_CHANNEL_WS2812;
    HAL_Delay(10);

    HAL_Delay(100);
    /*灭*/
    SetFlowBoardColor(WS2812_OFF, led_flow);
    DMA_CHANNEL_FLOW;
    HAL_Delay(10);
    AroundColor_Set(ws2812, WS2812_OFF, WS2812_LEN, ALL);
    DMA_CHANNEL_WS2812;
    HAL_Delay(10);

    HAL_Delay(100);


}

void WS2812_ALL_OFF() {
    SetFlowBoardColor(WS2812_OFF, led_flow);
    DMA_CHANNEL_FLOW;
    HAL_Delay(10);
    AroundColor_Set(ws2812, WS2812_OFF, WS2812_LEN, ALL);
    DMA_CHANNEL_WS2812;
    HAL_Delay(10);

}

/**
 * 初始化灯条的状态，将所有的灯条全部关闭，灯带信号的带来。
 */
void WS2812_Init() {
    //初始化数组
    memset(led_flow, 0, (BOARD_LENGTH + 1) * 24);
    memset(ws2812, 0, (WS2812_LEN + 1) * 24);
    //设置箭头的颜色
    SetFlowColor(WS2812_OFF, led_flow);
    //初始换四周的灯带
    AroundColor_Init(ws2812, WS2812_OFF, WS2812_LEN);
    DMA_CHANNEL_FLOW;
    DMA_CHANNEL_WS2812;

}

unsigned int abs(unsigned int a, unsigned b) {
    unsigned int error = a - b;
    return a > b ? a - b : b - a;
}

/**
 * 更新灯珠的状态
 */
unsigned char spi_data[1] = {0};
extern uint32_t init_kg;
uint32_t kg = 0;
unsigned char windmill_state = OFF;

void WS2812_Load() {
    SetFlowBoardColor(color, led_flow);
    DMA_CHANNEL_FLOW;
    HAL_Delay(10);
    // AroundColor_Set(ws2812, color, WS2812_LEN, DOWN);
    while (1);
}


void WS2812_Load1() {

    /**
     * 等待接收spi的数据，如果接收到了数据，就进入READY模式。否则除了等待spi数据，什么事都不做（OFF状态）。
     */
    if (HAL_SPI_Receive_IT(&hspi1, spi_data, 1) == HAL_OK) {
        switch (spi_data[0]) {
            case 0x83:
                color = WS2812_RED;
                //AroundColor_Set(ws2812, color, WS2812_LEN, DOWN);
                break;
            case 0x81:
                color = WS2812_BLUE;
                // AroundColor_Set(ws2812, color, WS2812_LEN, DOWN);
                break;
            case 0x85:
                WS2812_ALL_OFF();

                windmill_state = OFF;
                break;
            case 0x8F:
                AroundColor_Set(ws2812, color, WS2812_LEN, DOWN);
                DMA_CHANNEL_WS2812;

                LoadFlowColor(color);
                DMA_CHANNEL_FLOW;
                AroundColor_Set(ws2812, color, WS2812_LEN, DOWN);
                windmill_state = READY_HIT;
                break;
            case 0x80:
                windmill_state = BLINK;
                break;
        }
        /*设置四周灯条的颜色*/
        DMA_CHANNEL_WS2812;
    }
    //  OLED_ShowInt(0, 0, windmill_state);
    switch (windmill_state) {
        case OFF:
            /*Do nothing but wait for spi data*/
            break;
        case READY_HIT:
            /*每200ms刷新一次箭头*/
            if (Get_Times_Flow() > 200) {
                LoadFlowColor(color);
                DMA_CHANNEL_FLOW;
                Set_Times_Flow(0);
            }

            /*读取传感器的数据，判断扇叶是否被击中，如果击中，进入HIT状态*/
            //kg = HX711_ReadCount();
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET) {
                windmill_state = HIT;
                OLED_ShowInt(0, 0, kg);

                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
            }
            break;
        case HIT:
            /*被击中之后，所有灯都亮起来*/
            SetFlowBoardColor(color, led_flow);
            DMA_CHANNEL_FLOW;
            HAL_Delay(10);
            AroundColor_Set(ws2812, color, WS2812_LEN, ALL);
            DMA_CHANNEL_WS2812;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
            break;
        case BLINK:
            Blink_led(color);
            windmill_state = OFF;
            break;
        default:
            break;
    }

}



