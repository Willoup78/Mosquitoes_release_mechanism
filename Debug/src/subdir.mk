################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hal_msp.c \
../src/main.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c \
../src/uart_printf.c 

OBJS += \
./src/hal_msp.o \
./src/main.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o \
./src/uart_printf.o 

C_DEPS += \
./src/hal_msp.d \
./src/main.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d \
./src/uart_printf.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F446RETx -DNUCLEO_F446RE -DSTM32F4 -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/inc" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/CMSIS/core" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/CMSIS/device" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/HAL_Driver/Inc/Legacy" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/HAL_Driver/Inc" -I"C:/Users/William/Documents/Projects/Mosquitoes_release_mechanism_soft/Mos/Utilities/STM32F4xx-Nucleo" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


