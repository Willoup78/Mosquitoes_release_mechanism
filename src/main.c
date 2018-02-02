/**
  ******************************************************************************
  * @file    main.c
  * @author  William PONSOT
  * @version V1.0
  * @date    11-April-2017
  * @brief   Default main function.
  ******************************************************************************
*/

//hello
			
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "uart_printf.h"
#include "pwm_input.h"
#include "rtc.h"
#include "adc_input.h"
#include "stepper.h"
#include "lcd.h"
#include "uart2drone.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

//static void EXTI1_IRQHandler_Config(void);
extern UART_HandleTypeDef Uart3Handle;
extern UART_HandleTypeDef UartHandle;
UART_HandleTypeDef huart1;

/* Private functions ---------------------------------------------------------*/
static void MX_USART1_UART_Init(void);
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{



	HAL_Init();
	/* Configure the system clock to 180 MHz */
	SystemClock_Config();
	//EXTI1_IRQHandler_Config();
	//EXTI15_10_IRQHandler_Config();

	uart_printf_init();

	MX_USART1_UART_Init();
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);


	rtc_init();
	rtc_set_alarm();


	adc_init();

	stepper_init();
	stepper_run(0);
	stepper_set_speed(0);

	//Added JG
	uart3_init();
	__HAL_UART_ENABLE_IT(&Uart3Handle, UART_IT_RXNE);

//	uint16_t sensor_value = 1015;
//	uint8_t hi_byte = sensor_value >> 8;  //enthält dann 0x43
//	uint8_t lo_byte = (uint8_t)(sensor_value & 0x00ff); //enthält 0x21
//	uint8_t data[] = {1, hi_byte, lo_byte};
//	//uint8_t data[] = {1, 4, 7};
//	HAL_UART_Transmit(&Uart3Handle, (uint8_t *)data, 3, 0xFF);

//	stepper_run(1);
//	stepper_set_speed(3);

	int once = 1;
	while (1)
	{

		if(once){
					once = 0;
					printf("Hello world!\n\r");

					if (HAL_UART_GetState(&huart1) == HAL_UART_STATE_READY)
					{
						printf("USART1 ready!\n\r");
					}

//
//					//send something on uart1
//					uint8_t *ch = "hello";
//					HAL_UART_Transmit(&huart1, &ch, 1, 100);
//
//					stepper_set_speed(20, BACKWARD);
				}

	}

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows:
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);


   /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


/**
  * @brief  Configures EXTI lines 2 (connected to PB3 pin) in interrupt mode
  * @param  None
  * @retval None
  */
/*static void EXTI1_IRQHandler_Config(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}*/

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

	//__HAL_RCC_GPIOB_CLK_ENABLE();

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

