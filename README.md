# stm_term

## Dependencies

Dependencies are included as submodules, see [cloning](#cloning)

### Cloning ###

``` bash
git clone --recursive https://github.com/hohfchns/stm_term
cd stm_term
git submodule init --recursive
```

### Usage

``` c
// WARNING: override the printf() functionality
#include "stm_term.h"
// ...

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  TERM_TXCB(huart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  TERM_RXCB(huart);
}

{
  // In init
  TERM_Init(&yourhuart);
}

{
  // In update loop
  const char* cmdbuf = 0;
  // Reroutes the pointer and returns 1 if a command was received, else 0
  if (TERM_PullCmd(&cmdbuf))
  {
    // Parse your commands out of the received string
    ParseCommands(cmdbuf);
  }
}
```
