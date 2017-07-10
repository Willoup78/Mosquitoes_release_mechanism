/**
  ******************************************************************************
  * @file    rtc.c
  * @author  William PONSOT
  * @version V1.0
  * @date    20-April-2017
  * @brief   Functions configure and read real time clock (RTC)
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#include "adc_input.h"
#include "lcd.h"
#include "uart_printf.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef RtcHandle;

/* Private function prototypes -----------------------------------------------*/
void rtc_calendar_config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init the RTC
  * @param  None
  * @retval None
  */
void rtc_init(void)
{
	/*##-1- Configure the RTC peripheral #######################################*/
	RtcHandle.Instance = RTC;
	RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
	RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
	RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	HAL_RTC_Init(&RtcHandle);

	/*##-2- Check if Data stored in BackUp register1: No Need to reconfigure RTC#*/
	if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) != 0x32F2) //Meaning that the RTC has not be configured yet !
	{
	  rtc_calendar_config();
	}
}

/**
  * @brief  Configure the date and time of the RTC calendar
  * @param  None
  * @retval None
  */
void rtc_calendar_config(void)
{
	RTC_DateTypeDef sdatestructure;
	RTC_TimeTypeDef stimestructure;

	/*##-1- Configure the Date #################################################*/
	sdatestructure.Year = 0x17;
	sdatestructure.Month = RTC_MONTH_APRIL;
	sdatestructure.Date = 0x20;
	sdatestructure.WeekDay = RTC_WEEKDAY_THURSDAY;
	HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD);

	/*##-2- Configure the Time #################################################*/
	stimestructure.Hours = 0x00;
	stimestructure.Minutes = 0x00;
	stimestructure.Seconds = 0x00;
	stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
	stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&RtcHandle,&stimestructure,RTC_FORMAT_BCD);

	/*##-3- Writes a data in a RTC Backup data Register1 #######################*/
	HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2); //To avoid reconfiguring the RTC. This register is reset when power off !
}

/**
  * @brief  Read the date and time and stored it inside the Time structure.
  * @param  time: pointer to a Time structure
  * @retval None
  */
void rtc_calendar_read(Time* time)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);

  time->years = sdatestructureget.Year;
  time->months = sdatestructureget.Month;
  time->date = sdatestructureget.Date;
  time->weekday = sdatestructureget.WeekDay;
  time->hours = stimestructureget.Hours;
  time->minutes = stimestructureget.Minutes;
  time->seconds = stimestructureget.Seconds;
}

/**
  * @brief  Configure the alarm to call a callback function every second
  * @param  None
  * @retval None
  */
void rtc_set_alarm(void)
{
	RTC_AlarmTypeDef RtcAlarm;

	RtcAlarm.Alarm = RTC_ALARM_A;
	RtcAlarm.AlarmMask = RTC_ALARMMASK_ALL;  //Configure interruptions every second
	HAL_RTC_SetAlarm_IT(&RtcHandle,&RtcAlarm,RTC_FORMAT_BCD);
}

/**
  * @brief  Alarm callback
  * @param  hrtc : RTC handle
  * @retval None
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	adc_belt_speed();

	lcd_display(NONE);

	uart_sd_recording();

}

void rtc_delay_sec(uint8_t delay)
{
	Time time;
	uint8_t entry_time;
	uint8_t exit_time;

	rtc_calendar_read(&time);
	entry_time = time.seconds;

	exit_time = entry_time + delay;

	if (exit_time > 60)
	{
		exit_time = exit_time - 60;
	}

	while (time.seconds != exit_time)
	{
		rtc_calendar_read(&time);
	}

}
