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