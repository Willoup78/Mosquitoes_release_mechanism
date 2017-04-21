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
ADC_HandleTypeDef AdcHandle;
DMA_HandleTypeDef DmaHandle;
uint32_t AdcBuffer[ADC_BUFFER_LENGTH];
uint16_t value[NB_ADC_SENSOR];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init the ADC and DMA to allow direct transfer from the ADC to the
  * 		DMA without interrupting the CPU
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
	AdcHandle.Init.ScanConvMode          = ENABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
	AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode disabled to have only 1 conversion at each conversion trig */
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
	AdcHandle.Init.NbrOfDiscConversion   = 0;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;        /* Conversion start trigged at each external event */
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	AdcHandle.Init.NbrOfConversion       = 2;
	AdcHandle.Init.DMAContinuousRequests = ENABLE;
	AdcHandle.Init.EOCSelection          = DISABLE;
	HAL_ADC_Init(&AdcHandle);

	/*##-2- Configure ADC regular channel ######################################*/
	sConfig.Channel      = ADC_CHANNEL_4;
	sConfig.Rank         = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	sConfig.Offset       = 0;
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

	sConfig.Channel      = ADC_CHANNEL_8;
	sConfig.Rank         = 2;
	sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	sConfig.Offset       = 0;
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig);

	/*##-3 Configure DMA ########################################################*/
	__DMA2_CLK_ENABLE();
	DmaHandle.Instance = DMA2_Stream4;
	DmaHandle.Init.Channel  = DMA_CHANNEL_0;
	DmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
	DmaHandle.Init.MemInc = DMA_MINC_ENABLE;
	DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	DmaHandle.Init.Mode = DMA_CIRCULAR;
	DmaHandle.Init.Priority = DMA_PRIORITY_HIGH;
	DmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	DmaHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	DmaHandle.Init.MemBurst = DMA_MBURST_SINGLE;
	DmaHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;
	HAL_DMA_Init(&DmaHandle);

	__HAL_LINKDMA(&AdcHandle, DMA_Handle, DmaHandle);
	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

	/*##-4 Start ADC ###########################################################*/
	HAL_ADC_Start_DMA(&AdcHandle, AdcBuffer, ADC_BUFFER_LENGTH);
}

/**
  * @brief  Extract the data from the buffer value[] where the adc values are stored at high speed
  * @param  sensor_value : array with the same size as value to store the data in
  * @retval None
  */
void adc_read_data(uint16_t* sensor_value)
{
	for (uint8_t i=0; i<NB_ADC_SENSOR; i++)
	{
		sensor_value[i] = value[i];
	}
}

/**
  * @brief  Conversion complete callback in non blocking mode. In our configuration,
  * 		the callback is called when the DMA buffer is full.
  * @param  AdcHandle : AdcHandle handle
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
	for (uint8_t i=0; i<NB_ADC_SENSOR; i++)
	{
		value[i] = AdcBuffer[i];
	}
}

