/**
  ******************************************************************************
  * @file    pwm_input.c
  * @author  William PONSOT
  * @version V1.0
  * @date    11-April-2017
  * @brief   Functions to measure a PWM in input
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "pwm_input.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
	/* Timer handler declaration */
TIM_HandleTypeDef        TimHandle;
	/* Timer Input Capture Configuration Structure declaration */
TIM_IC_InitTypeDef       sConfig;
	/* Slave configuration structure */
TIM_SlaveConfigTypeDef   sSlaveConfig;
	/* Captured Value */
__IO uint32_t            uwIC2Value = 0;
	/* Duty Cycle Value */
__IO uint32_t            uwDutyCycle = 0;
	/* Frequency Value */
__IO uint32_t            uwFrequency = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init the timer to catch the frequency and duty cycle of an PWM
  * @param  None
  * @retval None
  */
void pwm_input_init(void)
{
	  /*##-1- Configure the TIM peripheral #######################################*/
	  /* Set TIMx instance */
	  TimHandle.Instance = TIMx;
	  TimHandle.Init.Period = 0xFFFF;
	  TimHandle.Init.Prescaler = 0;
	  TimHandle.Init.ClockDivision = 0;
	  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	  HAL_TIM_IC_Init(&TimHandle);

	  /*##-2- Configure the Input Capture channels ###############################*/
	  /* Common configuration */
	  sConfig.ICPrescaler = TIM_ICPSC_DIV1;
	  sConfig.ICFilter = 0;

	  /* Configure the Input Capture of channel 1 */
	  sConfig.ICPolarity = TIM_ICPOLARITY_FALLING;
	  sConfig.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	  HAL_TIM_IC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);

	  /* Configure the Input Capture of channel 2 */
	  sConfig.ICPolarity = TIM_ICPOLARITY_RISING;
	  sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	  HAL_TIM_IC_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2);

	  /*##-3- Configure the slave mode ###########################################*/
	  /* Select the slave Mode: Reset Mode */
	  sSlaveConfig.SlaveMode     = TIM_SLAVEMODE_RESET;
	  sSlaveConfig.InputTrigger  = TIM_TS_TI2FP2;
	  HAL_TIM_SlaveConfigSynchronization(&TimHandle, &sSlaveConfig);

	  /*##-4- Start the Input Capture in interrupt mode ##########################*/
	  HAL_TIM_IC_Start_IT(&TimHandle, TIM_CHANNEL_2);

	  /*##-5- Start the Input Capture in interrupt mode ##########################*/
	  HAL_TIM_IC_Start_IT(&TimHandle, TIM_CHANNEL_1);

}

/**
  * @brief  Get the frequency and the dutycycle
  * @param  frequency: pointer to an uint32_t that contains the frequency value
  * @param  dutycyle: pointer to an uint32_t that contains the dutycycle value
  * @retval None
  */
void pwm_input_get_value (uint32_t* frequency, uint32_t* dutycycle)
{
	*frequency = uwFrequency;
	*dutycycle = uwDutyCycle;
}

/**
  * @brief  Input Capture callback in non blocking mode
  * @param  htim: TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    /* Get the Input Capture value */
    uwIC2Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

    if (uwIC2Value != 0)
    {
      /* Duty cycle computation */
      uwDutyCycle = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1)) * 100) / uwIC2Value;

      /* uwFrequency computation
      TIM1 counter clock = (RCC_Clocks.HCLK_Frequency) */
      uwFrequency = (HAL_RCC_GetHCLKFreq()) / uwIC2Value;
    }
    else
    {
      uwDutyCycle = 0;
      uwFrequency = 0;
    }
  }
}

