################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../isotp-c/isotp.c 

OBJS += \
./isotp-c/isotp.o 

C_DEPS += \
./isotp-c/isotp.d 


# Each subdirectory must supply rules for building sources it contributes
isotp-c/%.o isotp-c/%.su isotp-c/%.cyclo: ../isotp-c/%.c isotp-c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/UIT/HK1_2024-2025/CD_Nhung/Lab4/Lab04_SensorTester/isotp-c" -I"C:/Users/Admin/UIT/HK1_2024-2025/CD_Nhung/Lab4/Lab04_SensorTester/CanTP_Handler" -I"C:/Users/Admin/UIT/HK1_2024-2025/CD_Nhung/Lab4/Lab04_SensorTester/Diagnostic_Services" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-isotp-2d-c

clean-isotp-2d-c:
	-$(RM) ./isotp-c/isotp.cyclo ./isotp-c/isotp.d ./isotp-c/isotp.o ./isotp-c/isotp.su

.PHONY: clean-isotp-2d-c

