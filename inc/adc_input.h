/**
  ******************************************************************************
  * @file    adc_input.h
  * @author  William PONSOT
  * @version V1.0
  * @date    21-April-2017
  * @brief   Functions to handle the ADC conversion from sensors
  ******************************************************************************
*/

#ifndef ADC_INPUT_H_
#define ADC_INPUT_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "hal_msp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define ADC_BUFFER_LENGTH 8192
#define NB_ADC_SENSOR 2 //Number of sensor inputs

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void adc_init(void);
void adc_read_data(uint16_t* sensor_value);


#endif /* ADC_INPUT_H_ */
