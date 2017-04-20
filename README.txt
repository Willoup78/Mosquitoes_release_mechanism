/**
  * @file    README.txt 
  * @author  William PONSOT
  * @version V1.0
  * @date    11-April-2017
  * @brief   Description of the Mosquitoes release mechanism software.
  ******************************************************************************
 
 The program is working with the Nucleo evalutation board with the STM32F446RE 
 microcontroller.
 
 /* Description of the files -------------------------------------------------*/
 
 --> stm32f4xx_it (.c and .h)
 The file contains all the functions to handle the interruptions. The functions make 
 the link with the Handler inside the HAL library.
 
 --> hal_msp (.c and .h)
 The file define the low hardware ressource for all the hal ports
 
 --> uart_printf (.c and .h)
 The file contains all the functions to enable the printf communication through 
 the USB. The serial port has the following parameters :
 	- baudrate = 9600
 	- data lentgth = 8bits
 	- stop bit = 1bit
 	- parity = odd
 To read the data, RealTerm (hyperterminal) software is used. The above parameters
 the right serial port need to be enter. To find the serial port of the nucleo board,
 go to Device Manager and find the device. 
 
 --> pwm_input (.c and .h)
 The file contains all the functions to be able to read the frequency and the duty cycle 
 of a PWM signal. To acheive this, a timer is set up.
 
 --> rtc (.c and .h)
 The file contains all the functions to be able to use the Real Time Clock (RTC).
 An alarm is set to call a callback everysecond. This allows us to measure external
 signal at a given rate.
 
 
 