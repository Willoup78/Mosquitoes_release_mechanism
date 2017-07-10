/**
  ******************************************************************************
  * @file    rtc.h
  * @author  William PONSOT
  * @version V1.0
  * @date    20-April-2017
  * @brief   Functions configure and read real time clock (RTC)
  ******************************************************************************
*/

#ifndef RTC_H_
#define RTC_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "hal_msp.h"

/* Exported types ------------------------------------------------------------*/
typedef struct Time Time;
struct Time
{
	uint8_t years;
	uint8_t months;
	uint8_t date;
	uint8_t weekday;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
};

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void rtc_init(void);
void rtc_calendar_read(Time* time);
void rtc_set_alarm(void);
void rtc_delay_sec(uint8_t delay);

#endif /* RTC_H_ */
