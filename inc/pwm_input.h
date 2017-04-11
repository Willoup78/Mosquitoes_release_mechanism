/**
  ******************************************************************************
  * @file    pwm_input.h
  * @author  William PONSOT
  * @version V1.0
  * @date    11-April-2017
  * @brief   Functions to measure a PWM in input
  ******************************************************************************
*/

#ifndef PWM_INPUT_H_
#define PWM_INPUT_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "hal_msp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void pwm_input_init(void);
void pwm_input_get_value (uint32_t* frequency, uint32_t* dutycycle);

#endif /* PWM_INPUT_H_ */
