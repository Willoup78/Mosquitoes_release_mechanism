/*
 * uart_printf.c
 *
 *  Created on: Apr 11, 2017
 *      Author: William
 */

/* Includes ------------------------------------------------------------------*/
#include "uart_printf.h"
#include "adc_input.h"
#include "rtc.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef UartHandle;
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init the uart to enable printf through the Serial port USB
  * @param  None
  * @retval None
  */
void uart_printf_init(void)
{
	  UartHandle.Instance          = USARTx;
	  UartHandle.Init.BaudRate     = 9600;
	  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
	  UartHandle.Init.StopBits     = UART_STOPBITS_1;
	  UartHandle.Init.Parity       = UART_PARITY_NONE;
	  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	  UartHandle.Init.Mode         = UART_MODE_TX_RX;
	  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	  HAL_UART_Init(&UartHandle);
}

void uart_sd_recording()
{
	uint16_t sensor_value[NB_ADC_SENSOR];
	Time time;

	rtc_calendar_read(&time);
	adc_read_data(sensor_value);

	printf("%d %d %d ", time.hours, time.minutes, time.seconds);

	for (uint8_t i=0 ; i<NB_ADC_SENSOR ; i++)
	{
		printf("%d ", sensor_value[i]);
	}
	printf("\n\r");
}

/**
  * @brief  Prototype to convert the printf function to uart
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
