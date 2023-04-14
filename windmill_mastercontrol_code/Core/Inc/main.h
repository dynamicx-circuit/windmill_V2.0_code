/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO5_Pin GPIO_PIN_10
#define GPIO5_GPIO_Port GPIOB
#define SPI_CS5_Pin GPIO_PIN_11
#define SPI_CS5_GPIO_Port GPIOB
#define GPIO1_Pin GPIO_PIN_12
#define GPIO1_GPIO_Port GPIOB
#define SPI_CS1_Pin GPIO_PIN_13
#define SPI_CS1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOB
#define GPIO2_Pin GPIO_PIN_9
#define GPIO2_GPIO_Port GPIOA
#define SPI_CS2_Pin GPIO_PIN_10
#define SPI_CS2_GPIO_Port GPIOA
#define key2_Pin GPIO_PIN_15
#define key2_GPIO_Port GPIOA
#define key1_Pin GPIO_PIN_3
#define key1_GPIO_Port GPIOB
#define GPIO3_Pin GPIO_PIN_4
#define GPIO3_GPIO_Port GPIOB
#define SPI_CS3_Pin GPIO_PIN_5
#define SPI_CS3_GPIO_Port GPIOB
#define I2C_CLK_Pin GPIO_PIN_6
#define I2C_CLK_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_GPIO_Port GPIOB
#define GPIO4_Pin GPIO_PIN_8
#define GPIO4_GPIO_Port GPIOB
#define SPI_CS4_Pin GPIO_PIN_9
#define SPI_CS4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define RED_FLAG 1
#define BLUE_FLAG -1

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
