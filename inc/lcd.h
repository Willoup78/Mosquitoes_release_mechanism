/**
  ******************************************************************************
  * @file    lcd.h
  * @author  William PONSOT
  * @version V1.0
  * @date    23-June-2017
  * @brief   Functions to print on the LCD screen (UART communication)
  ******************************************************************************
*/
#ifndef LCD_H_
#define LCD_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* Exported types ------------------------------------------------------------*/
#define LCD_DEGREE 0xDF
#define LCD_ARROW_LEFT 0x7F
#define LCD_ARROW_RIGHT 0x7E

enum LCD_cmd{
	RIGHT,
	LEFT,
	NONE,
};

enum LCD_state{
	INIT,
	START,
	FINISH,
	BELT_SPEED,
	TIME,
	TEMP1,
	TEMP2,
	TEMP3,
	TEMP4,
	TEMP5,
};

#define NB_STATUS 10

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void lcd_init(void);
void lcd_display_init(void);
void send_char(uint8_t character);
void send_string (char* string, uint8_t total_size);

void lcd_display_time ();
void lcd_display_belt_speed();
void lcd_display_temp (uint8_t temp_sensor);
void lcd_ask_belt_speed();
void lcd_change_status(uint16_t new_status);
void lcd_display_start();
void lcd_display_finish();

void lcd_display(uint8_t cmd);

char* itoa(int i, char b[]);
void stringtochar(char a[], char b[]);

#endif /* LCD_H_ */
