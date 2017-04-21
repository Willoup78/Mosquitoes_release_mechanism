/**
  ******************************************************************************
  * @file    adc_input.c
  * @author  William PONSOT
  * @version V1.0
  * @date    21-April-2017
  * @brief   Functions to handle the ADC conversion from sensors
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "adc_input.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef    AdcHandle;
uint16_t Adc_value = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init the ADC
  * @param  None
  * @retval None
  */
void adc_init(void)
{
	ADC_ChannelConfTypeDef sConfig;

	/*##-1- Configure the ADC peripheral #######################################*/
	AdcHandle.Instance          		 = ADCx;
	AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
	AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;
	AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
	AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode disabled to have only 1 conversion at each conversion trig */
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
	AdcHandle.Init.NbrOfDiscConversion   = 0;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;        /* Conversion start trigged at each external event */
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	AdcHandle.Init.NbrOfConversion       = 1;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.EOCSelection          = DISABLE;
	HAL_ADC_Init(&AdcHandle);

	/*##-2- Configure ADC regular channel ######################################*/
	sConfig.Channel      = ADC_CHANNEL_4;
	sConfig.Rank         = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig.Offset       = 0;
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

	/*##-3- Start the conversion process #######################################*/
	HAL_ADC_Start_IT(&AdcHandle);

}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
  /* Get the converted value of regular channel */
	Adc_value = HAL_ADC_GetValue(AdcHandle);
	printf("%d \n\r", Adc_value);
}
