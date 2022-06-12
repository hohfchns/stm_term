#ifndef INC_TERM_INPUT_H_
#define INC_TERM_INPUT_H_

#include "ring_buffer.h"
#include "main.h"

void INPUT_BindTXRB(RingBuffer* txrb);
void INPUT_BindPrintHuart(UART_HandleTypeDef* huart);

int _write(int fd, char* ptr, int len);

int UART_Transmit (UART_HandleTypeDef *huart, uint8_t *pData, size_t len);

#endif /* INC_TERM_INPUT_H_ */
