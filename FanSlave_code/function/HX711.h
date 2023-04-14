//
// Created by 86136 on 2023/3/13.
//

#ifndef FANSLAVE_CODE_HX711_H
#define FANSLAVE_CODE_HX711_H


#define HX711_SCK_HIGH     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)
#define HX711_SCK_LOW     HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)

unsigned long HX711_ReadCount(void);

#endif //FANSLAVE_CODE_HX711_H
