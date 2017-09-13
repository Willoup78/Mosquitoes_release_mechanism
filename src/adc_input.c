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
#include "stdlib.h"
#include "stepper.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef AdcHandle;
DMA_HandleTypeDef DmaHandle;
uint32_t AdcBuffer[ADC_BUFFER_LENGTH*NB_ADC_SENSOR]; //Need to be a multiple of NB_ADC_SENSOR to avoid shift at each end.
uint16_t value[NB_ADC_SENSOR];
float resistor_value[] =
{
	1000, 5,
	1100, 10.1,
	1200, 12,
	1300, 14.8,
	1400, 15,
	1500, 16,
	1700, 20.5,
};

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
	AdcHandle.Init.NbrOfConversion       = NB_ADC_SENSOR;
	AdcHandle.Init.DMAContinuousRequests = ENABLE;
	AdcHandle.Init.EOCSelection          = DISABLE;
	HAL_ADC_Init(&AdcHandle);

	/*##-2- Configure ADC regular channel ######################################*/

	sConfig.Channel      = ADC_CHANNEL_0;
	sConfig.Rank         = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	sConfig.Offset       = 0;
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig); //PA0

	sConfig.Channel      = ADC_CHANNEL_1;
	sConfig.Rank         = 2;
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	sConfig.Offset       = 0;
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig); //PA1

	sConfig.Channel      = ADC_CHANNEL_8;
	sConfig.Rank         = 3;
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	sConfig.Offset       = 0;
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig); //PB0

	sConfig.Channel      = ADC_CHANNEL_11;
	sConfig.Rank         = 4;
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	sConfig.Offset       = 0;
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig); //PC1

	sConfig.Channel      = ADC_CHANNEL_10;
	sConfig.Rank         = 5;
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	sConfig.Offset       = 0;
	HAL_ADC_ConfigChannel(&AdcHandle, &sConfig); //PC0


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



float adctotemp(uint16_t adc_value)
{
	uint16_t min_value = 4096; //4096 is the max value possible
	float temp;
	uint16_t diff;

	for (uint32_t i=0; i< (sizeof(resistor_value)/2); i++)
	{
		diff = abs((uint16_t)resistor_value[2*i] - adc_value);

		if(diff < min_value)
		{
			min_value = diff;
			temp = resistor_value[2*i+1];
		}
	}

	return temp;
}

/**
  * @brief  Extract the value from the potentiometer to get the belt speed
  * @param  None
  * @retval None
  */
void adc_belt_speed()
{
	uint16_t sensor_value[NB_ADC_SENSOR];
	uint16_t belt_speed;

	adc_read_data(sensor_value);

	belt_speed = sensor_value[0]; // [adc unit 0-4096]

	belt_speed = adc_round_up(belt_speed);

	stepper_set_target_belt_speed(belt_speed);

	if (belt_speed < 800)
	{
		stepper_set_speed(5);
	}
	else if (belt_speed < 2500)
	{
		stepper_set_speed(10);
	}
	else if (belt_speed < 4000)
	{
		stepper_set_speed(20);
	}



	//printf("Belt speed = %d\n\r",belt_speed);
}

/**
  * @brief  Round up the down decade
  * @param  adc_value :
  * @retval round_up_value :
  */
uint16_t  adc_round_up(uint16_t adc_value)
{
	uint16_t hundred;
	uint16_t round_up_value;

	hundred = adc_value / 100;

	round_up_value = hundred*100;

	return round_up_value;
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

