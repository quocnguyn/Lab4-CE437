################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Diagnostic_Services/DiagnosticDefine.c 

OBJS += \
./Diagnostic_Services/DiagnosticDefine.o 

C_DEPS += \
./Diagnostic_Services/DiagnosticDefine.d 


# Each subdirectory must supply rules for building sources it contributes
Diagnostic_Services/%.o Diagnostic_Services/%.su Diagnostic_Services/%.cyclo: ../Diagnostic_Services/%.c Diagnostic_Services/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/UIT/HK1_2024-2025/CD_Nhung/Lab4/Lab04_SensorTester/isotp-c" -I"C:/Users/Admin/UIT/HK1_2024-2025/CD_Nhung/Lab4/Lab04_SensorTester/CanTP_Handler" -I"C:/Users/Admin/UIT/HK1_2024-2025/CD_Nhung/Lab4/Lab04_SensorTester/Diagnostic_Services" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Diagnostic_Services

clean-Diagnostic_Services:
	-$(RM) ./Diagnostic_Services/DiagnosticDefine.cyclo ./Diagnostic_Services/DiagnosticDefine.d ./Diagnostic_Services/DiagnosticDefine.o ./Diagnostic_Services/DiagnosticDefine.su

.PHONY: clean-Diagnostic_Services

