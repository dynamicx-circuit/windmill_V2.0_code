

#include "communication.h"
#include "delay.h"
#include "hit_reaction.h"
#include "spi.h"

unsigned char spi_data[5] = {0x83, 0x81, 0x8F, 0x85, 0x80};
extern uint8_t rand_num_max;

void Spi_CS_HIGH(uint8_t CS_Pin) {
  switch (CS_Pin) {
  case 1:
    SPI_CS1_HIGH;
    break;
  case 2:
    SPI_CS2_HIGH;
    break;
  case 3:
    SPI_CS3_HIGH;
    break;
  case 4:
    SPI_CS4_HIGH;
    break;
  case 5:
    SPI_CS5_HIGH;
    break;
  default:
    break;
  }
}

void Spi_CS_LOW(uint8_t CS_Pin) {
  switch (CS_Pin) {
  case 1:
    SPI_CS1_LOW
    break;
  case 2:
    SPI_CS2_LOW
    break;
  case 3:
    SPI_CS3_LOW
    break;
  case 4:
    SPI_CS4_LOW
    break;
  case 5:
    SPI_CS5_LOW
    break;
  default:
    break;
  }
}

void SPI_Send_Data(unsigned char Cs_Pin, unsigned char data) {
  Spi_CS_LOW(Cs_Pin);
  delayus(10);
  HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
  Spi_CS_HIGH(Cs_Pin);
  delayus(10);
}

void SPI_Send_Color_All(unsigned char data) {
  for (int i = 1; i <= rand_num_max; i++) {
    SPI_Send_Data(i, data);
  }
  HAL_Delay(10);
}

void SPI_CS_Init() {
  for (int i = 1; i <= 5; i++) {
    Spi_CS_HIGH(i);
  }
}