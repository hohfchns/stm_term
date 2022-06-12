#include "term_input.h"

static RingBuffer* g_BoundTXRB;
static UART_HandleTypeDef* g_BoundHuart;

void INPUT_BindTXRB(RingBuffer* txrb)
{
  g_BoundTXRB = txrb;
}

void INPUT_BindPrintHuart(UART_HandleTypeDef* huart)
{
  g_BoundHuart = huart;
}

int _write(int fd, char* ptr, int len)
{
  UART_Transmit(g_BoundHuart, (uint8_t *) ptr, len);
  return len;
}

int UART_Transmit (UART_HandleTypeDef *huart, uint8_t *pData, size_t len)
{
  if (HAL_UART_Transmit_IT (huart, pData, len) != HAL_OK)
  {
    if (!RingBuffer_Write (g_BoundTXRB, pData, len))
    {
      return 0;
    }
  }

  return 1;
}



