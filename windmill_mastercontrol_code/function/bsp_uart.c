//
// Created by 86136 on 2023/5/9.
//

#include "bsp_uart.h"
#include "main.h"
#include "usart.h"
#include "communication.h"
#include "hit_reaction.h"
#include "fsm.h"

extern uint8_t rand_num_max;

uint8_t uart_data = 0;

void uart_IT(unsigned char *fsm_state) {
    HAL_UART_Receive_IT(&huart2, &uart_data, 1);
    if (uart_data == '1') {
        HAL_UART_Transmit(&huart2, "hello\n", 6, HAL_MAX_DELAY);
        for (int i = 1; i <= rand_num_max; i++) {
            SPI_Send_Data(i, spi_data[off]);
        }
        uart_data = 0;
    } else if (uart_data == '2') {
        *fsm_state = reset;
        uart_data = 0;
    }
}