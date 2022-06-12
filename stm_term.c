#include "stm_term.h"
#include "main.h"
#include <string.h>

static UART_HandleTypeDef* g_TermHuart;
static RingBuffer g_RxRB, g_TxRB;
static char g_CmdBuf[TERM_CMD_BUF_SIZE];
static int g_CmdIndex;
static char g_LastChar = 0;

int TERM_Init(UART_HandleTypeDef* huart)
{
  RingBuffer_Init(&g_RxRB);
  RingBuffer_Init(&g_TxRB);
  g_CmdBuf[0] = 0;
  g_CmdIndex = 0;
  g_TermHuart = huart;

  INPUT_BindPrintHuart(g_TermHuart);
  INPUT_BindTXRB(&g_TxRB);

  HAL_UART_Receive_IT(g_TermHuart, (uint8_t*)&g_LastChar, 1);

  return 1;
}

int TERM_PullCmd(const char** buf)
{
  if (g_CmdIndex == 0)
  {
    return 0;
  }

  RingBuffer_Read(&g_RxRB, (uint8_t*)g_CmdBuf, g_CmdIndex);

  g_CmdBuf[g_CmdIndex] = 0; // End the command string
  g_CmdIndex = 0; // Reset the index to prepare for next command

//  strcpy(buf, g_CmdBuf);
  *buf = g_CmdBuf;
  return 1;
}

void TERM_RXCB(UART_HandleTypeDef* huart)
{
  // Only act on term huart
  if (huart != g_TermHuart)
  {
    return;
  }

  if (g_LastChar == ENTER_CODE)
  {
    // this is the end of the input
    // notify that we ready to handle new command
    g_CmdIndex = RING_BUFFER_SIZE - RingBuffer_FreeSpace(&g_RxRB);
    UART_Transmit(g_TermHuart, (uint8_t*)"\r\n", 2);
  }
  else if (g_LastChar == BACKSPACE_CODE)
  {
//      // Hack that pulls the buffer and returns it without the last character
//      int size = RING_BUFFER_SIZE - RingBuffer_FreeSpace(&g_RxRB);
//      char tempBuf[RING_BUFFER_SIZE];
//      RingBuffer_Read(&g_RxRB, (uint8_t*)tempBuf, size);
//      RingBuffer_Write(&g_RxRB, (uint8_t*)tempBuf, size - 1);

      RingBuffer_RemoveFromHead(&g_RxRB, 1);

      UART_Transmit(g_TermHuart, (uint8_t*)"\b \b", 3);
  }
  else // Regular character
  {
    // put char to rx buffer
    if (RingBuffer_Write(&g_RxRB, (uint8_t*)&g_LastChar, 1) != 1)
    {
      // no more space in buffer, clear it
      RingBuffer_Init(&g_RxRB);
    }

    // we can echo it because echo will use interrupt and will not stop this interrupt
    UART_Transmit(g_TermHuart, (uint8_t*)&g_LastChar, 1);
  }

  // USER DEFINED IN main.h !
#ifdef F401
  if ((g_TermHuart->Instance->SR & USART_SR_ORE) != 0)
  {
    __HAL_UART_CLEAR_OREFLAG(g_TermHuart);
  }
#endif // def F401

#ifndef F401
  if ((g_TermHuart->Instance->ISR & USART_ISR_ORE) != 0)
  {
    __HAL_UART_CLEAR_OREFLAG(g_TermHuart);
  }
#endif // ndef F401

  HAL_UART_Receive_IT(g_TermHuart, (uint8_t*)&g_LastChar, 1);
}

void TERM_TXCB(UART_HandleTypeDef* huart)
{
  // Only act on term huart
  if (huart != g_TermHuart)
  {
    return;
  }

  uint8_t txData;
  if (!RingBuffer_IsEmpty(&g_TxRB))
  {
    RingBuffer_Read(&g_TxRB, &txData, 1);
    HAL_UART_Transmit_IT(g_TermHuart, &txData, 1);
  }
}











































