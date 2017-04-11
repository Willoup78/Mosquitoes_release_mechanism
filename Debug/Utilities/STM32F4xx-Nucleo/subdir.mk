################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/STM32F4xx-Nucleo/stm32f4xx_nucleo.c 

OBJS += \
./Utilities/STM32F4xx-Nucleo/stm32f4xx_nucleo.o 

C_DEPS += \
./Utilities/STM32F4xx-Nucleo/stm32f4xx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/STM32F4xx-Nucleo/%.o: ../Utilities/STM32F4xx-Nucleo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F446RETx -DNUCLEO_F446RE -DSTM32F4 -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/inc" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/CMSIS/core" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/CMSIS/device" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/HAL_Driver/Inc/Legacy" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/HAL_Driver/Inc" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/Utilities/STM32F4xx-Nucleo" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


