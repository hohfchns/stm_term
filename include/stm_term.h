#ifndef INC_STM_TERM_H_
#define INC_STM_TERM_H_

#include "main.h"
#include "term_input.h"  // OVERRIDES PRINTF

#define TERM_CMD_BUF_SIZE RING_BUFFER_SIZE

#define ENTER_CODE '\r'
#define BACKSPACE_CODE '\b'

// Returns if init was successful
int TERM_Init(UART_HandleTypeDef* huart_);
int TERM_PullCmd(const char** buf);

void TERM_RXCB(UART_HandleTypeDef* huart);
void TERM_TXCB(UART_HandleTypeDef* huart);

#endif /* INC_STM_TERM_H_ */
