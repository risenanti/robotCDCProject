################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__weak="__attribute__((weak))" -D__packed="__attribute__((__packed__))" -DUSE_HAL_DRIVER -DSTM32F411xE -I"C:/Users/Obelisk/Desktop/stm32f4discoworkspace/robotCDC/Inc" -I"C:/Users/Obelisk/Desktop/stm32f4discoworkspace/robotCDC/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Obelisk/Desktop/stm32f4discoworkspace/robotCDC/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Obelisk/Desktop/stm32f4discoworkspace/robotCDC/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Obelisk/Desktop/stm32f4discoworkspace/robotCDC/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/Obelisk/Desktop/stm32f4discoworkspace/robotCDC/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Users/Obelisk/Desktop/stm32f4discoworkspace/robotCDC/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Obelisk/Desktop/stm32f4discoworkspace/robotCDC/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/Obelisk/Desktop/stm32f4discoworkspace/robotCDC/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/Obelisk/Desktop/stm32f4discoworkspace/robotCDC/Drivers/CMSIS/Include"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


