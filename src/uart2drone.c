/**
  ******************************************************************************
  * @file    lcd.c
  * @author  William PONSOT
  * @version V1.0
  * @date    23-June-2017
  * @brief   Functions to print on the LCD screen (UART communication)
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
//#include "lcd.h"
//#include "rtc.h"
//#include "adc_input.h"
#include "stepper.h"
/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef Uart3Handle;
//uint8_t status = INIT;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void uart3_init()
{
	Uart3Handle.Instance          = USART3;
	Uart3Handle.Init.BaudRate     = 9600;
	Uart3Handle.Init.WordLength   = UART_WORDLENGTH_8B;
	Uart3Handle.Init.StopBits     = UART_STOPBITS_1;
	Uart3Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	Uart3Handle.Init.Mode         = UART_MODE_TX_RX;
	Uart3Handle.Init.OverSampling = UART_OVERSAMPLING_16;
	Uart3Handle.Init.Parity       = UART_PARITY_NONE;
	HAL_UART_Init(&Uart3Handle);

	//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//USART_Cmd(USART3, ENABLE);
	//while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET)
	//    ;
}

//void lcd_display_init()
//{
//	uint8_t x;
//	const char *incoming_string = "   WeRobotics   ";
//	for(x = 0 ; incoming_string[x] != '\0' ; x++)
//		send_char(incoming_string[x]);
//
//	incoming_string = "     V2.1       ";
//	for(x = 0 ; incoming_string[x] != '\0' ; x++)
//			send_char(incoming_string[x]);
//
//}
//
//void send_char(uint8_t character)
//{
//	uint8_t character_char = character;
//	HAL_UART_Transmit(&Uart3Handle, (uint8_t *)& character_char, 1, 0xFFFF);
//}
//
//void send_string (char* string, uint8_t total_size)
//{
//	uint8_t s, i;
//	uint8_t size = 0;
//	char *space = " ";
//
//	for(s = 0 ; string[s] != '\0' ; s++)
//		size++;
//
//	for(i=0; i<(total_size-size)/2 ; i++)
//		send_char(space[0]);
//
//	for(s = 0 ; string[s] != '\0' ; s++)
//	{
//		send_char(string[s]);
//		i++;
//	}
//
//	for(i; i<total_size; i++)
//		send_char(space[0]);
//}
//
//void lcd_display_time ()
//{
//	uint8_t x;
//
//	Time time;
//
//	char time_hours[16];
//	char time_minutes[16];
//	char time_seconds[16];
//
//	char *title = "Time";
//	char *space = " ";
//	char *unit_hours = "h ";
//	char *unit_minutes = "m ";
//	char *unit_seconds = "s";
//
//	rtc_calendar_read(&time);
//	itoa(time.hours,time_hours);
//	itoa(time.minutes,time_minutes);
//	itoa(time.seconds,time_seconds);
//
//	send_char(LCD_ARROW_LEFT);
//	send_string(title,14);
//	send_char(LCD_ARROW_RIGHT);
//
//	send_string(time_hours,2);
//	send_string(unit_hours,2);
//
//	send_string(time_minutes,2);
//	send_string(unit_minutes,2);
//
//	send_string(time_seconds,2);
//	send_string(unit_seconds,2);
//
//	for (x=12; x<16; x++)
//		send_char(space[0]);
//
//}
//
//void lcd_ask_belt_speed()
//{
//	char belt_speed[16];
//	char *title = "Belt speed ?";
//	char *unit = "mm/s";
//	char *space = " ";
//	uint16_t speed;
//	uint8_t letter = 0;
//
//	speed = stepper_get_target_belt_speed();
//
//	itoa(speed, belt_speed);
//
//	send_string(title,16);
//
//	if (speed < 100)
//	{
//		send_string(belt_speed,2);
//		letter = 2;
//	}
//
//	else if (speed < 1000)
//	{
//		send_string(belt_speed,3);
//		letter = 3;
//	}
//
//	else if (speed < 10000)
//	{
//		send_string(belt_speed,4);
//		letter = 4;
//	}
//
//
//	send_string(unit,4);
//	letter = letter + 4;
//
//	for (uint8_t x=letter; x<16; x++)
//		send_char(space[0]);
//
//}
//
//void lcd_display_belt_speed()
//{
//	char belt_speed[16];
//	char *title = "Belt speed";
//	char *unit = "mm/s";
//	char *space = " ";
//	uint16_t speed;
//	uint8_t letter = 0;
//
//	speed = stepper_get_target_belt_speed();
//
//	itoa(speed, belt_speed);
//
//	send_char(LCD_ARROW_LEFT);
//	send_string(title,14);
//	send_char(LCD_ARROW_RIGHT);
//
//	if (speed < 100)
//	{
//		send_string(belt_speed,2);
//		letter = 2;
//	}
//	else if (speed < 1000)
//	{
//		send_string(belt_speed,3);
//		letter = 3;
//	}
//	else if (speed < 10000)
//	{
//		send_string(belt_speed,4);
//		letter = 4;
//	}
//
//	send_string(unit,4);
//	letter = letter + 4;
//
//	for (uint8_t x=letter; x<16; x++)
//		send_char(space[0]);
//}
//
//void lcd_display_temp (uint8_t temp_sensor)
//{
//	uint8_t x;
//	char *space = " ";
//	char *unit = "C";
//
//	char title[14];
//	char temp[16];
//	uint16_t temp_value;
//	uint16_t adc_values[NB_ADC_SENSOR];
//
//	adc_read_data(adc_values);
//
//	switch (temp_sensor)
//	{
//		case TEMP1:
//		{
//			stringtochar(title, "T Ambiant");
//			temp_value = adctotemp(adc_values[0]);
//			break;
//		}
//
//		case TEMP2:
//		{
//			stringtochar(title, "T Container B");
//			temp_value = adctotemp(adc_values[1]);
//			break;
//		}
//
//		case TEMP3:
//		{
//			stringtochar(title, "T Container M");
//			temp_value = adctotemp(adc_values[2]);
//			break;
//		}
//
//		case TEMP4:
//		{
//			stringtochar(title, "T End belt");
//			temp_value = adctotemp(adc_values[3]);
//			break;
//		}
//
//		case TEMP5:
//		{
//			stringtochar(title, "T Outlet");
//			temp_value = adctotemp(adc_values[4]);
//			break;
//		}
//	}
//
//	itoa(temp_value, temp);
//
//	send_char(LCD_ARROW_LEFT);
//	send_string(title,14);
//	send_char(LCD_ARROW_RIGHT);
//
//	send_string(temp,3);
//	send_char(LCD_DEGREE);
//	send_string(unit,1);
//
//	for (x=5; x<16; x++)
//		send_char(space[0]);
//
//}
//
//void lcd_display_start()
//{
//	char *title = "START !";
//	char *message = "Press one arrow";
//
//	send_string(title,16);
//	send_string(message,16);
//}
//
//void lcd_display_finish()
//{
//	char *title = "FINISH !";
//	char *message = "----------------";
//
//	send_string(title,16);
//	send_string(message,16);
//
//}
//void lcd_display(uint8_t cmd)
//{
//
//	switch(cmd)
//	{
//		case NONE: break;
//		case RIGHT:
//		{
//		 status++;
//		 if(status > NB_STATUS-1)
//			 status = 3;
//		 break;
//		}
//		case LEFT:
//		{
//		 status--;
//		 if(status < 3)
//			 status = NB_STATUS-1;
//		 break;
//		}
//
//	}
//
//	switch(status)
//	{
//		case INIT:
//		{
//			lcd_ask_belt_speed();
//			break;
//		}
//		case START:
//		{
//			lcd_display_start();
//			break;
//		}
//		case FINISH:
//		{
//			lcd_display_finish();
//			break;
//		}
//		case BELT_SPEED:
//		{
//			lcd_display_belt_speed();
//			break;
//		}
//		case TIME:
//		{
//			lcd_display_time();
//			break;
//		}
//		case TEMP1:
//		{
//			lcd_display_temp(TEMP1);
//			break;
//		}
//
//		case TEMP2:
//		{
//			lcd_display_temp(TEMP2);
//			break;
//		}
//
//		case TEMP3:
//		{
//			lcd_display_temp(TEMP3);
//			break;
//		}
//
//		case TEMP4:
//		{
//			lcd_display_temp(TEMP4);
//			break;
//		}
//
//		case TEMP5:
//		{
//			lcd_display_temp(TEMP5);
//			break;
//		}
//
//
//	}
//}
//
//void lcd_change_status(uint16_t new_status)
//{
//	status = new_status;
//}
//
//char* itoa(int i, char b[]){
//    char const digit[] = "0123456789";
//    char* p = b;
//    if(i<0){
//        *p++ = '-';
//        i *= -1;
//    }
//    int shifter = i;
//    do{ //Move to where representation ends
//        ++p;
//        shifter = shifter/10;
//    }while(shifter);
//    *p = '\0';
//    do{ //Move back, inserting digits as u go
//        *--p = digit[i%10];
//        i = i/10;
//    }while(i);
//    return b;
//}
//
//void stringtochar(char a[], char b[])
//{
//	uint8_t i=0;
//
//	for (uint8_t j=0; j<14 ;j++)
//	{
//		a[j] = ' ';
//	}
//
//	do
//	{
//		a[i] = b[i];
//		i++;
//	}while(b[i] != '\0');
//
//	a[i+1] = '\0';
//}
