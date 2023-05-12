/**
  ******************************************************************************
  * @file    ws2812.c
  * @author  kade
  * @brief   This file provides some function to control the ws2812 led light strip
  ******************************************************************************
  *
 [..]
   (#) �ڸ��ļ���ʹ������·DMA������WS2812��һ·��������8*32�ĵư壬һ·�����������ܵĵƴ���

   (#) �ڵ���֮ǰ����Ҫ�ȳ�ʼ���ƴ���״̬���ڴ˽��ƴ�ȫ���رա�

   (#) ��ʼ�����֮�󣬽���׼��״̬���ȴ����巢�͵��źţ��Դ���ȷ����

   (#) ����led�Ƶ�״̬��
       (++) ����8*32�ư��״̬�����ӵȴ������״̬ʱ��8*32�ư����ʾ�����ļ�ͷ������Ҷ������ʱ�������ư�����
       (++) �������ܵƴ���״̬�������ڵȴ������״̬ʱ���ϲ��ֵĵƴ����������²��ֵĵƴ��������
            ������֮�����ܵĵƴ���������

   (#) 2023.4.19 ���£���糵�ĴӰ���������Ҫ�ټ���һ��Բ���ĵư壬�����ϵĵ�������Ϊ440�š�
       ����ram�������ˣ�����׼����Բ���ĺͺ����ܵƴ������鸴��ʹ�á�
   */

#include"ws2812.h"
#include<string.h>
#include"main.h"
#include"tim.h"
#include "spi.h"
#include "HX711.h"
#include "clock.h"

unsigned int color = WINDMILL_INIT_COLOR;
unsigned int color_low = WS2812_RED_LOW;

unsigned char led_flow[BOARD_LENGTH + 1][24];
unsigned char ws2812[WS2812_LEN + 1][24];
///*�糵��ͷ������*/
unsigned char flow[48] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 1, 0,
        0, 1, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 1, 1, 0, 0, 0
};

/**
 * ���õ���led����ɫ
 * @param led   led�����ڵ����飬ʹ��24λ����ʾ��һ����ʹ�õ�����Ҫ��24λ
 * @param color Ҫ���õ���ɫ
 */
inline void SetSingleColor(unsigned char led[24], unsigned int color) {
    for (unsigned char i = 0; i < 24; i++) {
        if ((color >> i) & 1) led[i] = WS2812_H;
        else led[i] = WS2812_L;
    }
}

/**
 * ����һ���Ƶ���ɫ
 * @param color
 * @param leds led�����ڵ����飬ʹ��24λ����ʾ��һ����ʹ�õ�����Ҫ��24λ
 * @param length �ƴ��ĳ���
 */
void SetColor(unsigned char leds[][24], unsigned int color, unsigned int length) {
    for (unsigned int i = 0; i < length; i++)
        SetSingleColor(leds[i], color);
}

/**
 * ���ü�ͷ����ɫ
 * @param color ��ɫ
 * @param leds ��ͷ��Ӧ������
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

/**
 * ����������ͷ���ӵ���ɫ
 * @param color
 * @param leds
 */
void SetFlowBoardColor(unsigned long color, unsigned char leds[BOARD_LENGTH][24]) {
    for (unsigned long i = 0; i < BOARD_LENGTH; i++) {
        SetSingleColor(leds[i], color);
    }
}

/**
 * ���¼�ͷ��ʹ������
 */
void LoadFlowColor(unsigned long color) {
    uint8_t flow_buffer[40];
    memcpy(flow_buffer, flow, 40);
    memcpy(flow, flow + 40, 8);
    memcpy(flow + 8, flow_buffer, 40);
    SetFlowColor(color, led_flow);
}

/**
 * ��ʼ�����ܵ������ɫ
 * @param leds
 * @param color
 * @param length
 */
void AroundColor_Init(unsigned char leds[][24], unsigned int color, unsigned int length) {
    for (int i = 0; i < length; i++) {
        SetSingleColor(leds[i], color);
    }
}


void AroundColor_Set(unsigned char leds[][24], unsigned int color, unsigned int length, enum ws2812_state_e state) {
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
                SetSingleColor(leds[i], color);
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

void CircleColor_Set(enum ws2812_circle_state_e state) {
    if (state == CIRCLE_HIT) {
        unsigned int sand_num = rands(10);
        if (sand_num == 1) {
            for (int n = 1; n <= 10; n++) {
                if (n == 1 || (n % 2) == 0) {
                    for (int i = 4 * n * (n - 1); i < 4 * n * (n + 1); i += 1) {
                        SetSingleColor(ws2812[i], color);
                    }
                } else {
                    for (int i = 4 * n * (n - 1); i < 4 * n * (n + 1); i += 1) {
                        SetSingleColor(ws2812[i], WS2812_OFF);
                    }
                }
            }
        } else {
            for (int n = 1; n <= 10; n++) {
                if (n == sand_num) {
                    for (int i = 4 * n * (n - 1); i < 4 * n * (n + 1); i += 1) {
                        SetSingleColor(ws2812[i], color);
                    }
                } else {
                    for (int i = 4 * n * (n - 1); i < 4 * n * (n + 1); i += 1) {
                        SetSingleColor(ws2812[i], WS2812_OFF);
                    }
                }
            }

        }
        HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_2, (uint32_t *) ws2812, (WS2812_LEN + 1) * 24);
        HAL_Delay(30);
        for (int i = 0; i < WS2812_LEN; ++i) {
            SetSingleColor(ws2812[i], WS2812_OFF);
        }
    } else if (state == CIRCLE_OFF) {
        for (int i = 0; i < WS2812_LEN; ++i) {
            SetSingleColor(ws2812[i], WS2812_OFF);
        }
        HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_2, (uint32_t *) ws2812, (WS2812_LEN + 1) * 24);
        HAL_Delay(20);
    } else if (state == CIRCLE_ON) {
        //�Ƚ����еƹر�
        for (int i = 0; i < WS2812_LEN; ++i) {
            SetSingleColor(ws2812[i], WS2812_OFF);
        }
//        HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_2, (uint32_t *) ws2812, (WS2812_LEN + 1) * 24);
//        HAL_Delay(10);

        /*nΪ������dΪÿһ�����е���ļ��*/
        for (int n = 1; n <= 10; n++) {
            int d = 0;
            if (n == 9 || n == 6 || n == 3) {
                d = 1;
            } else if (n == 5 || n == 7 || n == 8 || n == 10) {
                d = (8 * n) / 4;
            } else {
                continue;
            }
            for (int i = 4 * n * (n - 1) + (d / 2); i < 4 * n * (n + 1); i += d) {
                SetSingleColor(ws2812[i], color);
            }
        }
        HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_2, (uint32_t *) ws2812, (WS2812_LEN + 1) * 24);
        HAL_Delay(20);
        for (int i = 0; i < WS2812_LEN; ++i) {
            SetSingleColor(ws2812[i], WS2812_OFF);
        }
    }
}

void Blink_led(unsigned int color) {

    /*��*/
    SetFlowBoardColor(color, led_flow);
    DMA_CHANNEL_FLOW;
    HAL_Delay(10);
    AroundColor_Set(ws2812, color, WS2812_LEN, ALL);
    DMA_CHANNEL_WS2812;
    HAL_Delay(10);

    HAL_Delay(100);
    /*��*/
    SetFlowBoardColor(WS2812_OFF, led_flow);
    DMA_CHANNEL_FLOW;
    HAL_Delay(10);
    AroundColor_Set(ws2812, WS2812_OFF, WS2812_LEN, ALL);
    DMA_CHANNEL_WS2812;
    HAL_Delay(10);

    HAL_Delay(100);


}

/**
 * �ر����еĵ�
 */
void WS2812_ALL_OFF() {
    SetFlowBoardColor(WS2812_OFF, led_flow);
    DMA_CHANNEL_FLOW;
    HAL_Delay(10);
    AroundColor_Set(ws2812, WS2812_OFF, WS2812_LEN, ALL);
    DMA_CHANNEL_WS2812;
    HAL_Delay(10);
    CircleColor_Set(CIRCLE_OFF);

}

/**
 * ��ʼ��������״̬�������еĵ���ȫ���رգ��ƴ��źŵĴ�����
 */
void WS2812_Init() {
    //��ʼ������
    memset(ws2812, 0, (WS2812_LEN + 1) * 24);

    memset(led_flow, 0, (BOARD_LENGTH + 1) * 24);
    //���ü�ͷ����ɫ
    SetFlowColor(WS2812_OFF, led_flow);
    //��ʼ�����ܵĵƴ�
    AroundColor_Init(ws2812, WS2812_OFF, WS2812_LEN);
    DMA_CHANNEL_FLOW;
    DMA_CHANNEL_WS2812;
    DMA_CHANNEL_CIRCLE;

}

/**
 * ���µ����״̬
 */
unsigned char spi_data[1] = {0};
extern uint32_t init_kg;
uint32_t kg = 0;
unsigned char windmill_state = OFF;

void WS2812_Load1() {
//    SetFlowBoardColor(color, led_flow);
//    AroundColor_Set(ws2812, color, WS2812_LEN, ALL);
//
//    DMA_CHANNEL_FLOW;
//    DMA_CHANNEL_WS2812;

//    SetSingleColor(ws2812[3],WS2812_RED);
    //DMA_CHANNEL_FLOW;
    while (1) {
        CircleColor_Set(CIRCLE_ON);
        HAL_Delay(1000);
        CircleColor_Set(CIRCLE_HIT);
        HAL_Delay(1000);
        CircleColor_Set(CIRCLE_OFF);
        HAL_Delay(1000);


    }

    // AroundColor_Set(ws2812, color, WS2812_LEN, DOWN);
    //while (1);
}


void WS2812_Load() {

    /**
     * �ȴ�����spi�����ݣ�������յ������ݣ��ͽ���READYģʽ��������˵ȴ�spi���ݣ�ʲô�¶�������OFF״̬����
     */
    if (HAL_SPI_Receive_IT(&hspi1, spi_data, 1) == HAL_OK) {
        switch (spi_data[0]) {
            case 0x83:
                color = WS2812_RED;
                color_low = WS2812_RED_LOW;
                //AroundColor_Set(ws2812, color, WS2812_LEN, DOWN);
                break;
            case 0x81:
                color = WS2812_BLUE;
                color_low = WS2812_BLUE_LOW;
                // AroundColor_Set(ws2812, color, WS2812_LEN, DOWN);
                break;
            case 0x85:
                WS2812_ALL_OFF();

                windmill_state = OFF;
                break;
            case 0x8F:
                CircleColor_Set(CIRCLE_ON);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
                AroundColor_Set(ws2812, color, WS2812_LEN, DOWN);
                DMA_CHANNEL_WS2812;

                LoadFlowColor(color);
                DMA_CHANNEL_FLOW;
                windmill_state = READY_HIT;
                break;
            case 0x80:
                windmill_state = BLINK;
                break;
        }
        /*�������ܵ�������ɫ*/
        DMA_CHANNEL_WS2812;
    }
    //  OLED_ShowInt(0, 0, windmill_state);
    switch (windmill_state) {
        case OFF:
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET) {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
                HAL_Delay(10);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

            }
            /*Do nothing but wait for spi data*/
            break;
        case READY_HIT:
            /*ÿ200msˢ��һ�μ�ͷ*/
            if (Get_Times_Flow() > 200) {
                LoadFlowColor(color);
                DMA_CHANNEL_FLOW;
                Set_Times_Flow(0);
            }

            /*��ȡ�����������ݣ��ж���Ҷ�Ƿ񱻻��У�������У�����HIT״̬*/
            //kg = HX711_ReadCount();
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET) {
                windmill_state = HIT;

                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

            }
            break;
        case HIT:
            /*������֮�����еƶ�������*/
            CircleColor_Set(CIRCLE_HIT);
            SetFlowBoardColor(color, led_flow);
            DMA_CHANNEL_FLOW;
            HAL_Delay(10);
            AroundColor_Set(ws2812, color, WS2812_LEN, ALL);
            DMA_CHANNEL_WS2812;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
            windmill_state = OFF;

            break;
        case BLINK:
            Blink_led(color);
            windmill_state = OFF;
            break;
        default:
            break;
    }

}



