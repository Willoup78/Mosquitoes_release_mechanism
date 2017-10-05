/**
  ******************************************************************************
  * @file    stepper.h
  * @author  William PONSOT
  * @version V1.0
  * @date    24-April-2017
  * @brief   Functions to handle stepper motor
  ******************************************************************************
*/

#ifndef STEPPER_H_
#define STEPPER_H_

/* Includes ------------------------------------------------------------------*/
#include "motorcontrol.h"
#include "L6474.h"
#include "stm32f4xx_nucleo_ihm01a1.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define STEPPER_ACC 400
#define STEPPER_MODE L6474_STEP_SEL_1_8
#define STEPPER_CURRENT_MAX 3375 //(375;6000) with 375 step increment
#define STEPPER_SPEED_MIN 0
#define STEPPER_SPEED_MAX 150
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void stepper_init(void);
void stepper_set_speed(uint16_t speed_rpm);
uint16_t stepper_rpm2step (float speed_rpm);
uint16_t stepper_get_target_belt_speed();
void stepper_set_target_belt_speed(uint16_t value);
void stepper_run(bool start);


#endif /* STEPPER_H_ */
