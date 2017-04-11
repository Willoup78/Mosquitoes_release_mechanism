################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f446xx.s 

OBJS += \
./startup/startup_stm32f446xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo %cd%
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/inc" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/CMSIS/core" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/CMSIS/device" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/HAL_Driver/Inc/Legacy" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/HAL_Driver/Inc" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/Utilities/STM32F4xx-Nucleo" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


