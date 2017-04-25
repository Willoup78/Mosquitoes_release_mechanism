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
 
 --> adc_input (.c and .h)
The file contains all the function to initialize the ADC with a DMA storage. The value 
are stored at very high frequency inside the DMA buffer. Then when the buffer is full
the ADC callback is called and the first values from the buffer are extracted in value[]. The DMA 
buffer is needed because the ADC can run in background without interrupting the CPU at each 
completed sequence of conversion (conversion of all the channels). This allows to run the ADC
at whatever frequency without risking to have an ADC interruption when an other task is performed.
Finally, the data from the intermediate buffer value[] can be read with a dedicated function (adc_read_data).
To add other input, the following steps need to be done :
	- configure the channel in adc_init (adc_input.c)
	- configure the GPIO in HAL_ADC_MspInit (hal_msp.c)
	- change the number of sensor constant NB_ADC_SENSOR (adc_input.h)
	
--> STEPPER DRIVER files (in order of abstraction, HIGH to LOW)
	--> stepper (.c and .h)
	All the functions needed to init and set up a speed to the stepper
	
	--> motorcontrol (.c and .h) and motor.h
	Functions to handle differents kind of motor, highest level of abstraction of the motor library of ST
	
	--> L6474 (.c and .h) and L6474_target_config.h
	Functions to handle the driver on the nucleo expansion board
	
	--> stm32f4xx_nucleo_ihm01a1 (.c and .h)
	Low hardware functions to make the link between the STM32F4 board and the expansion board
	
 
 
 