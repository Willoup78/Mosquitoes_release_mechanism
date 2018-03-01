/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  William PONSOT
  * @version V1.0
  * @date    11-April-2017
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#ifdef USE_RTOS_SYSTICK
#include <cmsis_os.h>
#endif
#include "stm32f4xx_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef TimHandle; //First declaration in pwm_input.c
extern RTC_HandleTypeDef RtcHandle; //First declaration in rtc.c
extern ADC_HandleTypeDef AdcHandle; //First declaration in adc_input.c
extern DMA_HandleTypeDef DmaHandle; //First declaration in adc_input.c

//NEW
extern TIM_HandleTypeDef hTimPwm1;
extern TIM_HandleTypeDef hTimPwm2;
extern TIM_HandleTypeDef hTimPwm3;

extern UART_HandleTypeDef Uart3Handle;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef UartHandle;
uint16_t currentChar[2];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
#ifdef USE_RTOS_SYSTICK
	osSystickHandler();
#endif
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIMx_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
}

/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_Alarm_IRQHandler(void)
{
	HAL_RTC_AlarmIRQHandler(&RtcHandle);
}

/**
  * @brief  This function handles ADC interrupt request.
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&AdcHandle);
}

/**
  * @brief  This function handles DMA2 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream4_IRQHandler()
{
	HAL_DMA_IRQHandler(&DmaHandle);
}


// NEW

/**
  * @brief  This function handles interrupt for External lines 1
  * @param  None
  * @retval None
  */

/*void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}*/

/**
  * @brief  This function handles interrupt for External lines 1
  * @param  None
  * @retval None
  */

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
}





/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&hTimPwm2);
}

/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&hTimPwm1);
}

/**
  * @brief  This function handles TIM4 interrupt request.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&hTimPwm3);
}


/**
* @brief This function handles USART1 global interrupt.
this function is called everytime a byte is sent over the line
hence we need to check if the end of the message has been reached / or agree on a length of transmission
*/
void USART3_IRQHandler(void)
{
	/* USER CODE BEGIN USART1_IRQn 0 */

	/* USER CODE END USART1_IRQn 0 */
	HAL_UART_IRQHandler(&Uart3Handle);

	/* USER CODE BEGIN USART1_IRQn 1 */

	//send to usart2
	//uint8_t buff_ACK[30] = "Receiving command from drone to update motor speed!";
	//HAL_UART_Transmit(&UartHandle, buff_ACK, 30, 100);

	//uint8_t buff_ACK[12] = "Interrupt 3 ";
	//HAL_UART_Transmit(&UartHandle, buff_ACK, 12, 100);

	//uint8_t buff_RX[10];
	// this function will write the received bytes into the buffer
	//returns HAL_OK
	HAL_UART_Receive_IT(&Uart3Handle, currentChar, 1);

	//HAL_UART_Transmit(&UartHandle, currentChar, 6, 100);

}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	//print message
	// currentChar contains now the message
	HAL_UART_Transmit(&UartHandle, currentChar, 1, 100);

	if(huart->Instance == USART3){

		//print message
		uint8_t buff_ACK[10] = "Int UART3: ";
		HAL_UART_Transmit(&UartHandle, buff_ACK, 10, 100);

		//printf("\n\r");

		//stepper_set_speed(10);
		uint8_t data[] = {10, 0, 0};
		uint8_t message0[15] = "Motor speed 0\n";
		uint8_t message1[15] = "Motor speed 1\n";
		uint8_t message2[15] = "Motor speed 2\n";
		uint8_t message3[15] = "Motor speed 3\n";
		uint8_t message4[15] = "Communication check\n";
		uint8_t message5[15] = "Command unknown\n";

		switch(currentChar[0]){
			case 4:
				stepper_run(0);
				stepper_set_speed(0);
				HAL_UART_Transmit(&UartHandle, message0, 15, 100);
				//inform OSDK
				HAL_UART_Transmit(&Uart3Handle, (uint8_t *)data, 3, 0xFF);
				break;
			case 1:
				stepper_run(1);
				stepper_set_speed(1);
				HAL_UART_Transmit(&UartHandle, message1, 15, 100);
				//inform OSDK
				data[0] = 11;
				HAL_UART_Transmit(&Uart3Handle, (uint8_t *)data, 3, 0xFF);
				break;
			case 2:
				stepper_run(1);
				stepper_set_speed(2);
				HAL_UART_Transmit(&UartHandle, message2, 15, 100);
				//inform OSDK
				data[0] = 12;
				HAL_UART_Transmit(&Uart3Handle, (uint8_t *)data, 3, 0xFF);
				break;
			case 3:
				stepper_run(1);
				stepper_set_speed(3);
				HAL_UART_Transmit(&UartHandle, message3, 15, 100);
				//inform OSDK
				data[0] = 13;
				HAL_UART_Transmit(&Uart3Handle, (uint8_t *)data, 3, 0xFF);
				break;
			case 5:
				HAL_UART_Transmit(&UartHandle, message4, 15, 100);
				//inform OSDK
				data[0] = 15;
				HAL_UART_Transmit(&Uart3Handle, (uint8_t *)data, 3, 0xFF);
				break;
			default:
				HAL_UART_Transmit(&UartHandle, message5, 15, 100);
				data[0] = 16;
				//inform OSDK
				HAL_UART_Transmit(&Uart3Handle, (uint8_t *)data, 3, 0xFF);
				break;
		}
	}

}

/**
* @brief This function handles USART1 global interrupt.
this function is called everytime a byte is sent over the line
hence we need to check if the end of the message has been reached / or agree on a length of transmission
*/
void USART1_IRQHandler(void)
{
	/* USER CODE BEGIN USART1_IRQn 0 */

	/* USER CODE END USART1_IRQn 0 */
	HAL_UART_IRQHandler(&huart1);

	/* USER CODE BEGIN USART1_IRQn 1 */
	uint8_t buff_ACK[12] = "Interrupt 1 ";
	HAL_UART_Transmit(&UartHandle, buff_ACK, 12, 100);

	//uint8_t buff_RX[10];
	// this function will write the received bytes into the buffer
	//returns HAL_OK
	HAL_UART_Receive_IT(&huart1, currentChar, 1);

	//HAL_UART_Transmit(&UartHandle, currentChar, 6, 100);

}





