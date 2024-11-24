################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include isotp-c/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Diagnostic_Services/Tester_Diagnostic/subdir.mk
-include Diagnostic_Services/ECU_Diagnostic/subdir.mk
-include Diagnostic_Services/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include CanTP_Handler/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Lab04_SensorTester
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Lab04_SensorTester.elf \

MAP_FILES += \
Lab04_SensorTester.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Lab04_SensorTester.list \

OBJCOPY_BIN += \
Lab04_SensorTester.bin \


# All Target
all: main-build

# Main-build Target
main-build: Lab04_SensorTester.elf secondary-outputs

# Tool invocations
Lab04_SensorTester.elf Lab04_SensorTester.map: $(OBJS) $(USER_OBJS) C:\Users\Admin\UIT\HK1_2024-2025\CD_Nhung\Lab4\Lab04_SensorTester\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Lab04_SensorTester.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\Admin\UIT\HK1_2024-2025\CD_Nhung\Lab4\Lab04_SensorTester\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Lab04_SensorTester.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Lab04_SensorTester.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Lab04_SensorTester.list"
	@echo 'Finished building: $@'
	@echo ' '

Lab04_SensorTester.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Lab04_SensorTester.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Lab04_SensorTester.bin Lab04_SensorTester.elf Lab04_SensorTester.list Lab04_SensorTester.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
