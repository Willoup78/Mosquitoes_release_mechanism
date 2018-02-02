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
extern UART_HandleTypeDef Uart3Handle;
int counter = 0;
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

	//rtc_calendar_read(&time);
	adc_read_data(sensor_value);

	//test: send to OnboardSDK: OK
	//uint8_t some_value = '2';
	//HAL_UART_Transmit(&Uart3Handle, (uint8_t *) &some_value, 1, 0xFFFF);


	//printf("%d %d %d ", time.hours, time.minutes, time.seconds);

	//for (uint8_t i=0 ; i<NB_ADC_SENSOR ; i++)
	int i = counter;
	{
		//uint8_t * dataPointer = &sensor_value[i];
		uint8_t sensor = i;
		printf("%d ", sensor);
		printf("%d ", sensor_value[i]);
		printf("%f \n", adctotemp(sensor_value[i]));
		uint8_t hi_byte = sensor_value[i] >> 8;  //enthält dann 0x43
		//printf("%d \n", hi_byte);
		uint8_t lo_byte = (uint8_t)(sensor_value[i] & 0x00ff); //enthält 0x21
		uint8_t data[] = {100+sensor, hi_byte, lo_byte};
		//printf("%d \n", lo_byte);
		HAL_UART_Transmit(&Uart3Handle, (uint8_t *)data, 3, 0xFF);
		//HAL_Delay(100);

		int nCount = 100;
		for(; nCount != 0; nCount--);


//		HAL_UART_Transmit(&Uart3Handle, (uint8_t *)&sensor, 1, 0xFF);
//		HAL_UART_Transmit(&Uart3Handle, (uint8_t *)&hi_byte, 1, 0xFF);
//		HAL_UART_Transmit(&Uart3Handle, (uint8_t *)&lo_byte, 1, 0XFF);

		//uint16_t Rx = (hi_byte << 8) | lo_byte;
		//printf("%d ", Rx);
//		HAL_UART_Transmit(&Uart3Handle, (uint8_t *)(sensor_value[i]), 2, 0);


		//this works OK
		//uint8_t some_value = sensor_value[i];
		//HAL_UART_Transmit(&Uart3Handle, (uint8_t *) &some_value, 1, 0xFFFF);
		//printf("%d ", some_value);
		//printf("%d ", sensor_value[i]);

	}
	counter++;
	if(counter >= 5){counter = 0;}
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
