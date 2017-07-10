/**
  ******************************************************************************
  * @file    stepper.c
  * @author  William PONSOT
  * @version V1.0
  * @date    24-April-2017
  * @brief   Functions to handle stepper motor
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stepper.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
L6474_Init_t gL6474InitParams =
{
	STEPPER_ACC,                       /// Acceleration rate in step/s2. Range: (0..+inf).
	STEPPER_ACC,                       /// Deceleration rate in step/s2. Range: (0..+inf).
    400,                               /// Maximum speed in step/s. Range: (30..10000].
    30,                                /// Minimum speed in step/s. Range: [30..10000).
    2000,                               /// Torque regulation current in mA. (TVAL register) Range: 31.25mA to 4000mA.
    STEPPER_CURRENT_MAX,               /// Overcurrent threshold (OCD_TH register). Range: 375mA to 6000mA.
    L6474_CONFIG_OC_SD_ENABLE,        /// Overcurrent shutwdown (OC_SD field of CONFIG register).
    L6474_CONFIG_EN_TQREG_TVAL_USED,   /// Torque regulation method (EN_TQREG field of CONFIG register).
	STEPPER_MODE,                      /// Step selection (STEP_SEL field of STEP_MODE register).
	L6474_SYNC_SEL_1_2,                /// Sync selection (SYNC_SEL field of STEP_MODE register).
    L6474_FAST_STEP_12us,              /// Fall time value (T_FAST field of T_FAST register). Range: 2us to 32us.
    L6474_TOFF_FAST_8us,               /// Maximum fast decay time (T_OFF field of T_FAST register). Range: 2us to 32us.
    3,                                 /// Minimum ON time in us (TON_MIN register). Range: 0.5us to 64us.
    21,                                /// Minimum OFF time in us (TOFF_MIN register). Range: 0.5us to 64us.
    L6474_CONFIG_TOFF_044us,           /// Target Swicthing Period (field TOFF of CONFIG register).
    L6474_CONFIG_SR_320V_us,           /// Slew rate (POW_SR field of CONFIG register).
    L6474_CONFIG_INT_16MHZ,            /// Clock setting (OSC_CLK_SEL field of CONFIG register).
    (L6474_ALARM_EN_OVERCURRENT      |
     L6474_ALARM_EN_THERMAL_SHUTDOWN |
     L6474_ALARM_EN_THERMAL_WARNING  |
     L6474_ALARM_EN_UNDERVOLTAGE     |
     L6474_ALARM_EN_SW_TURN_ON       |
     L6474_ALARM_EN_WRONG_NPERF_CMD)    /// Alarm (ALARM_EN register).
};

uint16_t target_speed;
uint16_t stepper_speed;

/* Private function prototypes -----------------------------------------------*/
static void MyFlagInterruptHandler(void);

/* Private functions ---------------------------------------------------------*/

void stepper_set_target_belt_speed(uint16_t adc_value)
{
	uint16_t scale = STEPPER_SPEED_MAX - STEPPER_SPEED_MIN;
	float divider = 4096 / scale;

	target_speed = (uint16_t)((float)adc_value / divider);
}

uint16_t stepper_get_target_belt_speed()
{
	return target_speed;
}

/**
  * @brief  Set the new speed [rpm] and go to this speed. Not depending of the step_mode
  * 		the motor goes to the right speed in rpm.
  * @param  speed_rpm: desired speed [rpm]
  * @retval speed_step: value of the speed [step/s]
  */
uint16_t stepper_rpm2step (float speed_rpm)
{
	float speed_step;
	motorStepMode_t step_mode;
	step_mode = L6474_GetStepMode(0);

	switch((uint8_t)step_mode)
	{
		case(STEP_MODE_FULL):
				speed_step = speed_rpm * 400  / 60;
				break;
		case(STEP_MODE_HALF):
				speed_step = speed_rpm * 800  / 60;
				break;
		case(STEP_MODE_1_4):
				speed_step = speed_rpm * 1600  / 60;
				break;
		case(STEP_MODE_1_8):
				speed_step = speed_rpm * 3200  / 60;
				break;
		case(STEP_MODE_1_16):
				speed_step = speed_rpm * 6400  / 60;
				break;
	}

	// Set up to MAX or MIN step/s if the value exceeds the range
	if(speed_step > 10000)
		speed_step = 10000;
	if(speed_step < 30)
		speed_step = 30;

	return (uint16_t)speed_step;
}

/**
  * @brief  Set the new speed [rpm] and go to this speed
  * @param  speed_rpm: new target speed [rpm]
  * @param  direction: choose the direction of the motor (FORWARD or BACKWARD)
  * @retval None
  */
void stepper_set_speed(uint16_t speed_rpm, motorDir_t direction)
{
	uint16_t speed_step;

	speed_step = stepper_rpm2step(speed_rpm);
	BSP_MotorControl_Run(0, direction);
	BSP_MotorControl_SetMaxSpeed(0, speed_step);
}

/**
  * @brief  Init the stepper motor
  * @param  None
  * @retval None
  */
void stepper_init()
{
	BSP_MotorControl_SetNbDevices(BSP_MOTOR_CONTROL_BOARD_ID_L6474, 1);
	BSP_MotorControl_Init(BSP_MOTOR_CONTROL_BOARD_ID_L6474, &gL6474InitParams);
	BSP_MotorControl_AttachFlagInterrupt(MyFlagInterruptHandler);
}

/**
  * @brief  This function is the User handler for the flag interrupt
  * @param  None
  * @retval None
  */
void MyFlagInterruptHandler(void)
{
  /* Get the value of the status register via the L6474 command GET_STATUS */
  uint16_t statusRegister = BSP_MotorControl_CmdGetStatus(0);

  /* Check HIZ flag: if set, power brigdes are disabled */
  if ((statusRegister & L6474_STATUS_HIZ) == L6474_STATUS_HIZ)
  {
    // HIZ state
    // Action to be customized
  }

  /* Check direction bit */
  if ((statusRegister & L6474_STATUS_DIR) == L6474_STATUS_DIR)
  {
    // Forward direction is set
    // Action to be customized
  }
  else
  {
    // Backward direction is set
    // Action to be customized
  }

  /* Check NOTPERF_CMD flag: if set, the command received by SPI can't be performed */
  /* This often occures when a command is sent to the L6474 */
  /* while it is in HIZ state */
  if ((statusRegister & L6474_STATUS_NOTPERF_CMD) == L6474_STATUS_NOTPERF_CMD)
  {
      // Command received by SPI can't be performed
     // Action to be customized
  }

  /* Check WRONG_CMD flag: if set, the command does not exist */
  if ((statusRegister & L6474_STATUS_WRONG_CMD) == L6474_STATUS_WRONG_CMD)
  {
     //command received by SPI does not exist
     // Action to be customized
  }

  /* Check UVLO flag: if not set, there is an undervoltage lock-out */
  if ((statusRegister & L6474_STATUS_UVLO) == 0)
  {

	  //printf("Undervoltage shunt down < 7.2V");
  }

  /* Check TH_WRN flag: if not set, the thermal warning threshold is reached (130C) */
  if ((statusRegister & L6474_STATUS_TH_WRN) == 0)
  {
	  //printf("Thermal warning 130C");
  }

  /* Check TH_SHD flag: if not set, the thermal shut down threshold is reached (160C)*/
  if ((statusRegister & L6474_STATUS_TH_SD) == 0)
  {
	  //printf("Thermal shunt down 160C");
  }

  /* Check OCD  flag: if not set, there is an overcurrent detection */
  if ((statusRegister & L6474_STATUS_OCD) == 0)
  {
	  //printf("Overcurrent detection\n\r");
  }
}
