//
// Created by 86136 on 2023/3/2.
//

#ifndef WINDMILL_MASTERCONTROL_CODE_COMMUNICATION_H
#define WINDMILL_MASTERCONTROL_CODE_COMMUNICATION_H

extern unsigned char spi_data[5];
enum spi_data_e { red, blue, readyhit, off, blink };

#define SPI_CS1_HIGH                                                           \
  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); }
#define SPI_CS1_LOW                                                            \
  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET); }

#define SPI_CS2_HIGH                                                           \
  { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); }
#define SPI_CS2_LOW                                                            \
  { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); }

#define SPI_CS3_HIGH                                                           \
  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); }
#define SPI_CS3_LOW                                                            \
  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); }

#define SPI_CS4_HIGH                                                           \
  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET); }
#define SPI_CS4_LOW                                                            \
  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET); }

#define SPI_CS5_HIGH                                                           \
  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); }
#define SPI_CS5_LOW                                                            \
  { HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET); }

#define SPI_CSS_HICH_ALL                                                       \
  {SPI_CS1_HIGH SPI_CS2_HIGH SPI_CS3_HIGH SPI_CS4_HIGH SPI_CS5_HIGH};
#define SPI_CSS_LOW_ALL                                                        \
  {SPI_CS1_LOW SPI_CS2_LOW SPI_CS3_LOW SPI_CS4_LOW SPI_CS5_LOW};

#define BLUE 0x83
#define RED 0x81

#define READYHIT 0x8F
#define HIT 0x80

void SPI_CS_Init();

void Spi_CS_HIGH(unsigned char CS_Pin);

void Spi_CS_LOW(unsigned char CS_Pin);

void SPI_Send_Data(unsigned char Cs_Pin, unsigned char data);

void SPI_Send_Color_All(unsigned char data);
#endif // WINDMILL_MASTERCONTROL_CODE_COMMUNICATION_H
