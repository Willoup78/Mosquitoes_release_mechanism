/**
  ******************************************************************************
  * @file    hal_msp.c
  * @author  William PONSOT
  * @version V1.0
  * @date    20-April-2017
  * @brief   Init of low hardware level.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "hal_msp.h"
#include "lcd.h"
#include "rtc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//NEW
extern void BSP_MotorControl_StepClockHandler(uint8_t deviceId);
extern void BSP_MotorControl_FlagInterruptHandler(void);
extern motorState_t BSP_MotorControl_GetDeviceState(uint8_t deviceId);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  TIMx_CLK_ENABLE();

  /* Enable GPIO channels Clock */
  TIMx_CHANNEL_GPIO_PORT();

  /* Configure  (TIMx_Channel) in Alternate function, push-pull and 100MHz speed */
  GPIO_InitStruct.Pin = GPIO_PIN_CHANNEL2;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF_TIMx;
  HAL_GPIO_Init(GPIO_PORT, &GPIO_InitStruct);

  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(TIMx_CC_IRQn, 0, 1);
  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIMx_CC_IRQn);
}

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)
	{
		GPIO_InitTypeDef  GPIO_InitStruct;

		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		USARTx_TX_GPIO_CLK_ENABLE();
		USARTx_RX_GPIO_CLK_ENABLE();

		/* Enable USARTx clock */
		USARTx_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = USARTx_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = USARTx_TX_AF;

		HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

		/* UART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = USARTx_RX_PIN;
		GPIO_InitStruct.Alternate = USARTx_RX_AF;

		HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
	}

	if(huart->Instance==USART3)
	{
		GPIO_InitTypeDef  GPIO_InitStruct;

		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		__HAL_RCC_GPIOC_CLK_ENABLE();

		/* Enable USARTx clock */
		__HAL_RCC_USART3_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	}
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)
	{
		/*##-1- Reset peripherals ##################################################*/
		USARTx_FORCE_RESET();
		USARTx_RELEASE_RESET();

		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART Tx as alternate function  */
		HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
		/* Configure UART Rx as alternate function  */
		HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
	}

	if(huart->Instance == USART3)
	{
		__HAL_RCC_USART3_FORCE_RESET();
		__HAL_RCC_USART3_RELEASE_RESET();

		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART Tx as alternate function  */
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10);
		/* Configure UART Rx as alternate function  */
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_11);



	}


}

/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example
  * @param hrtc: RTC handle pointer
  *
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *        the RTC clock source; in this case the Backup domain will be reset in
  *        order to modify the RTC Clock source, as consequence RTC registers (including
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-2- Configure LSE as RTC clock source ###################################*/
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /*##-3- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

  /*##-4- Configure the NVIC for RTC Alarm ###################################*/
   HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0x0F, 0);
   HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**
  * @brief RTC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
   __HAL_RCC_RTC_DISABLE();

  /*##-2- Disables the PWR Clock and Disables access to the backup domain ###################################*/
  HAL_PWR_DisableBkUpAccess();
  __HAL_RCC_PWR_CLK_DISABLE();

}

/**
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
	GPIO_InitTypeDef          GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* ADC1 Periph clock enable */
	__HAL_RCC_ADC1_CLK_ENABLE();
	/* Enable GPIO clock ****************************************/
	__HAL_RCC_GPIOA_CLK_ENABLE(); //Port A
	__HAL_RCC_GPIOB_CLK_ENABLE(); //Port B
	__HAL_RCC_GPIOC_CLK_ENABLE(); //Port C

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* ADC Channel GPIO pin configuration */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //PA0

	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //PA1

	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); //PA4

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); //PB0

	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); //PC1

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); //PC0

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); //PC2

	/*##-3- Configure the NVIC #################################################*/
	/* NVIC configuration for ADC interrupt */
	HAL_NVIC_SetPriority(ADCx_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADCx_IRQn);
}

/**
  * @brief ADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{

	/*##-1- Reset peripherals ##################################################*/
	ADCx_FORCE_RESET();
	ADCx_RELEASE_RESET();

	/*##-2- Disable peripherals and GPIO Clocks ################################*/
	/* De-initialize the ADC Channel GPIO pin */
	HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
}


//NEW

/**
  * @brief SPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param[in] hspi SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  if(hspi->Instance == SPIx)
  {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();
    /* Enable SPI clock */
    SPIx_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_MEDIUM;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;

    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIx_MISO_AF;

    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;

    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);
  }
}

/**
  * @brief SPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  * @param[in] hspi SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if(hspi->Instance == SPIx)
  {
    /*##-1- Reset peripherals ##################################################*/
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Configure SPI SCK as alternate function  */
    HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
    /* Configure SPI MISO as alternate function  */
    HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
    /* Configure SPI MOSI as alternate function  */
    HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
  }
}

/**
  * @brief PWM MSP Initialization
  * @param[in] htim_pwm PWM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_pwm->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM1)
  {
    /* Peripheral clock enable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM1_CLCK_ENABLE();

    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_PWM_1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MOTOR_CONTROL_BOARD_AFx_TIMx_PWM1;
    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_PWM_1_PORT, &GPIO_InitStruct);

    /* Set Interrupt Group Priority of Timer Interrupt*/
    HAL_NVIC_SetPriority(BSP_MOTOR_CONTROL_BOARD_PWM1_IRQn, 4, 0);

    /* Enable the timer global Interrupt */
    HAL_NVIC_EnableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM1_IRQn);
  }
  else if(htim_pwm->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM2)
  {
    /* Peripheral clock enable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM2_CLCK_ENABLE();

    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_PWM_2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = BSP_MOTOR_CONTROL_BOARD_AFx_TIMx_PWM2;
    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_PWM_2_PORT, &GPIO_InitStruct);

    /* Set Interrupt Group Priority of Timer Interrupt*/
    HAL_NVIC_SetPriority(BSP_MOTOR_CONTROL_BOARD_PWM2_IRQn, 4, 0);

    /* Enable the timer2 global Interrupt */
    HAL_NVIC_EnableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM2_IRQn);

  }
  else if(htim_pwm->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM3)
  {
    /* Peripheral clock enable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM3_CLCK_ENABLE();

    /* GPIO configuration */
    GPIO_InitStruct.Pin = BSP_MOTOR_CONTROL_BOARD_PWM_3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(BSP_MOTOR_CONTROL_BOARD_PWM_3_PORT, &GPIO_InitStruct);

    /* Set Interrupt Group Priority of Timer Interrupt*/
    HAL_NVIC_SetPriority(BSP_MOTOR_CONTROL_BOARD_PWM3_IRQn, 3, 0);

    /* Enable the timer global Interrupt */
    HAL_NVIC_EnableIRQ(BSP_MOTOR_CONTROL_BOARD_PWM3_IRQn);
  }
}

/**
  * @brief PWM MSP De-Initialization
  * @param[in] htim_pwm PWM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM1)
  {
    /* Peripheral clock disable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM1_CLCK_DISABLE();

    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_MOTOR_CONTROL_BOARD_PWM_1_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_1_PIN);

  }
  else if(htim_pwm->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM2)
  {
    /* Peripheral clock disable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM2_CLCK_DISABLE();

    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_MOTOR_CONTROL_BOARD_PWM_2_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_2_PIN);

  }
  else if(htim_pwm->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM3)
  {
    /* Peripheral clock disable */
    __BSP_MOTOR_CONTROL_BOARD_TIMER_PWM3_CLCK_DISABLE();

    /* GPIO Deconfiguration */
    HAL_GPIO_DeInit(BSP_MOTOR_CONTROL_BOARD_PWM_3_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_3_PIN);
  }
}

/**
  * @brief PWM Callback
  * @param[in] htim PWM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  if ((htim->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM1)&& (htim->Channel == BSP_MOTOR_CONTROL_BOARD_HAL_ACT_CHAN_TIMER_PWM1))
  {
    if (BSP_MotorControl_GetDeviceState(0) != INACTIVE)
    {
      BSP_MotorControl_StepClockHandler(0);
    }
  }
  if ((htim->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM2)&& (htim->Channel == BSP_MOTOR_CONTROL_BOARD_HAL_ACT_CHAN_TIMER_PWM2))
  {
    if (BSP_MotorControl_GetDeviceState(1) != INACTIVE)
    {
      BSP_MotorControl_StepClockHandler(1);
    }
  }
  if ((htim->Instance == BSP_MOTOR_CONTROL_BOARD_TIMER_PWM3)&& (htim->Channel == BSP_MOTOR_CONTROL_BOARD_HAL_ACT_CHAN_TIMER_PWM3))
  {
    HAL_GPIO_TogglePin(BSP_MOTOR_CONTROL_BOARD_PWM_3_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_3_PIN);
    if ((BSP_MotorControl_GetDeviceState(2) != INACTIVE)&&
        (HAL_GPIO_ReadPin(BSP_MOTOR_CONTROL_BOARD_PWM_3_PORT, BSP_MOTOR_CONTROL_BOARD_PWM_3_PIN) == GPIO_PIN_SET))
    {
      BSP_MotorControl_StepClockHandler(2);
    }
  }
}

/**
  * @brief External Line Callback
  * @param[in] GPIO_Pin pin number
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static bool start = 0;
	Time time;
	static uint8_t entry_time = 0;
	uint8_t current_time;
	uint8_t delay;

	static bool key_press = 0;

	if (GPIO_Pin == BSP_MOTOR_CONTROL_BOARD_FLAG_PIN)
	{
	BSP_MotorControl_FlagInterruptHandler();
	}

	if (GPIO_Pin == GPIO_PIN_13) // LEFT PC13
	{
		lcd_display(LEFT);
	}

	if (GPIO_Pin == GPIO_PIN_14) //RIGHT PB14
	{
		lcd_display(RIGHT);
	}

	if (GPIO_Pin == GPIO_PIN_15) //START/STOP PB15
	{
		rtc_calendar_read(&time);
		current_time = time.seconds;

		if (start == 1 && (current_time-entry_time) > 2)
		{
			lcd_change_status(FINISH);
		}

		if(start == 0)
		{
			entry_time = time.seconds;
			lcd_change_status(START);
			start = 1;
		}
	}
}
