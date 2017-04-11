/*
 * uart_printf.h
 *
 *  Created on: Apr 11, 2017
 *      Author: William
 */

#ifndef UART_PRINTF_H_
#define UART_PRINTF_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "hal_msp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void uart_printf_init(void);

#endif /* UART_PRINTF_H_ */
